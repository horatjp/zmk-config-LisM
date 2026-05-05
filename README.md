# LisM キーボード ファームウェア

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

自作キーボード LisMのファームウェアです。

## 生成されるファームウェア一覧

| ファームウェア名                                 | 説明                                    |
|---------------------------------------------- |---------------------------------------|
| `lism_left_peripheral_non_trackball.uf2`      | 左側 ペリフェラル 非トラックボール     |
| `lism_left_peripheral_trackball.uf2`          | 左側 ペリフェラル トラックボール       |
| `lism_right_central_non_trackball.uf2`        | 右側 セントラル 非トラックボール       |
| `lism_right_central_trackball.uf2`            | 右側 セントラル トラックボール         |
| `lism_right_central_non_trackball_studio.uf2` | 右側 セントラル 非トラックボール (ZMK Studio 対応) |
| `lism_right_central_trackball_studio.uf2`     | 右側 セントラル トラックボール (ZMK Studio 対応)   |
| `settings_reset-seeeduino_xiao_ble-zmk.uf2`   | 設定リセット用                        |

## ローカルビルド手順

GitHub Actionsでのビルドは毎回2分-3分かかりますが、ローカル環境では40秒〜1分で完了します。(PCスペックによって前後します)  
キーマップの変更など、少し試したいだけでも時間がかかるActionsに比べ、ローカルビルドなら素早く試行錯誤ができます。

### 必要なもの
- [Visual Studio Code](https://code.visualstudio.com/)
- [Docker Desktop](https://www.docker.com/products/docker-desktop/)
- VS Code拡張機能: [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)

### 手順
1.  **準備**  
    1.  このリポジトリをPCにcloneします。
    2.  Docker Desktopを起動します。
    3.  VS Codeでこのフォルダを開きます。
    4.  右下に表示される「Reopen in Container(コンテナーで再度開く)」ボタンをクリックします。  
        (初回は環境構築に時間がかかります)

2.  **ビルド（ファームウェア作成）**  
    VS Codeのターミナルで、以下のいずれかのコマンドを実行します。

    > [!TIP]
    > **並列ビルドで時間を短縮！**  
    > ビルドはCPUコアを最大限に活用して並列実行できます。  
    > 特に多くのファームウェアを一度にビルドする際に効果的です。  
    > 並列数は自動でCPUコア数に設定されますが、環境変数 `PARALLEL` で指定することも可能です（例: `PARALLEL=4 make all_p`）。

    - **すべてのファームウェアを並列で一度に作成する場合(ZMK Studioサポートなし):**
      ```bash
      make
      ```

    - **すべてのファームウェアを並列で一度に作成する場合(ZMK Studioサポートあり):**
      ```bash
      make all_studio_p
      ```

    - **すべてのファームウェアを逐次で一度に作成する場合(ZMK Studioサポートなし):**
      ```bash
      make all
      ```

    - **すべてのファームウェアを逐次で一度に作成する場合(ZMK Studioサポートあり):**
      ```bash
      make all
      ```

    - **作成するファームウェアを選びたい場合:**
      ```bash
      make single
      ```
      表示されたリストから、作成したいファームウェアの番号を入力してEnterキーを押します。  
      (キーマップ変更のみであれば、lism_right_xxxxのみでOKです)

    - **作成されたファームウェアを全削除:**
      ```bash
      make clean
      ```

3.  **完成**  
    ビルドが完了すると、`firmware_builds` フォルダの中に `.uf2` ファイルが作成されます。  
    このファイルをキーボードに書き込むことで、ファームウェアが更新されます。

### 更新方法

この設定ファイル（リポジトリ）や、キーボードファームウェアのコア部分であるZMK等が更新された場合の手順です。

1.  **最新版の取得**  
    VS Codeのターミナルで、以下のコマンドを実行して設定ファイルを最新の状態にします。
    ```bash
    git pull
    ```

2.  **依存関係の更新**  
    続けて、以下のコマンドを実行して、ファームウェアのコア部分を更新します。
    ```bash
    make setup-west
    ```
    *(内部で `west update` が実行され、必要なファイルが更新されます)*

3.  **ビルド**  
    更新が完了したら、上記「ビルド（ファームウェア作成）」の手順でファームウェアを再作成してください。

## セントラル／ペリフェラルの入れ替え

LisM は左右どちらでも「セントラル（Central）」と「ペリフェラル（Peripheral）」を選べます。  
既定は右セントラルですが、左セントラル構成も用意できます。
1. `build.yaml` の「Central = Left」ブロックを有効化。
1. `build.yaml` の「Central = Right」ブロックを無効化。
1. ビルドして書き込み

