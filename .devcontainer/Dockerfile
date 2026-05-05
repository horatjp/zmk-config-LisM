FROM docker.io/zmkfirmware/zmk-build-arm:stable

COPY .bashrc tmp
RUN mv /tmp/.bashrc ~/.bashrc

RUN apt-get update && apt-get install -y --no-install-recommends \
    yq \
    curl \
    && curl -fsSL https://deb.nodesource.com/setup_lts.x | bash - \
    && apt-get install -y nodejs \
    && rm -rf /var/lib/apt/lists/*

RUN npm install -g @anthropic-ai/claude-code
