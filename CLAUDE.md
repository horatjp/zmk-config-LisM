# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## ビルドコマンド

すべてのコマンドはDevcontainer内で実行すること（`west`と`yq`がDevcontainer内にのみインストールされているため）。

```bash
make                  # 全ファームウェアを並列ビルド（studio除く）
make all              # 全ファームウェアを逐次ビルド（studio除く）
make all_studio_p     # studio含む全ファームウェアを並列ビルド
make single           # インタラクティブにビルド対象を選択
make setup-west       # westワークスペースの初期化・更新
make clean            # firmware_builds/ を削除
```

並列数を指定する場合: `PARALLEL=4 make`

## アーキテクチャ

### ディレクトリ構造の概要

- `config/` — ZMKユーザー設定（キーマップ、コンフィグ、west.yml）
- `boards/shields/lism/` — LisMキーボード用シールド定義
- `snippets/` — ビルドバリアント定義（trackball/non-trackball × central/peripheral）
- `scripts/` — ビルドスクリプト群
- `_west/` — westワークスペース（Git管理外。`make setup-west`で生成）
- `firmware_builds/` — ビルド成果物の出力先（Git管理外）

### ビルドシステムの流れ

1. `build.yaml` がビルドマトリクスを定義（board × shield × snippet × cmake-args の組み合わせ）
2. `scripts/build-matrix.sh` が `build.yaml` を `yq` でパースし、各エントリを `west build` で実行
3. `scripts/lib/build-helpers.sh` が成果物（`.uf2` または `.bin`）を `firmware_builds/` にコピー
4. `west build` は `_west/` をワークスペースとして実行し、`zmk/app` をソースとしてビルド

### westの依存関係（config/west.yml）

- `zmk` v0.3.0（zmkfirmware公式）
- `zmk-driver-paw3222`（トラックボールドライバ）
- `zmk-rgbled-widget` v0.3（RGB LED）
- `zmk-feature-charge-indicator`（充電インジケータ）

### キーボード構成

LisMは左右分割キーボード。デフォルトは右側がCentral（BLEホスト側）、左側がPeripheral。

- Centralには `-DCONFIG_ZMK_SPLIT_ROLE_CENTRAL=y` が必要
- ZMK Studio対応版には `-DCONFIG_ZMK_STUDIO=y` と `studio-rpc-usb-uart` snippetが必要
- セントラル／ペリフェラルを入れ替える場合は `build.yaml` の該当ブロックをコメントアウト／アンコメント

**現在のハードウェア構成**: 左=水平ロータリーエンコーダ、右=トラックボール
→ 書き込むファームウェア: 左に `lism_left_peripheral_non_trackball`、右に `lism_right_central_trackball`

### ファームウェアの種類

| ファームウェア | 説明 | 書き込み対象 |
|---|---|---|
| `lism_left_peripheral_non_trackball` | 左側ペリフェラル、エンコーダ ★現構成 | 左 |
| `lism_left_peripheral_trackball` | 左側ペリフェラル、トラックボールあり | 左 |
| `lism_right_central_non_trackball` | 右側セントラル、エンコーダ | 右 |
| `lism_right_central_trackball` | 右側セントラル、トラックボールあり ★現構成 | 右 |
| `lism_right_central_non_trackball_studio` | 右側セントラル、トラックボールなし、ZMK Studio対応 | 右 |
| `lism_right_central_trackball_studio` | 右側セントラル、トラックボールあり、ZMK Studio対応 | 右 |
| `settings_reset-seeeduino_xiao_ble-zmk` | 設定リセット用（ペアリング情報消去など） | 左右両方 |

ZMK Studio対応版はメモリ使用量が増加するため非推奨。キーマップ変更には DYA Studio（ビルド不要）またはローカルビルドを推奨。

### キーマップ変更時のビルド

- キーマップ編集: `config/lism.keymap`
- ハードウェア構成変更: `boards/` または `snippets/` 内のファイル
- キーマップのみの変更であれば右側（Central）のファームウェアだけ書き直せばよい。`make single` でターゲットを絞ることで時間を短縮できる。

## DevcontainerとWestワークスペース

- ベースイメージ: `zmkfirmware/zmk-build-arm:stable`
- 追加ツール: `yq`、`curl`、Node.js、`claude-code`
- `postCreateCommand` で `setup-west.sh` が自動実行される
- `_west/config/west.yml` は `config/west.yml` へのシンボリックリンク
- `/root` はDockerボリューム（`lism-zmk-root-user`）にマウントされ、コンテナ再作成後も`_west`等が保持される
