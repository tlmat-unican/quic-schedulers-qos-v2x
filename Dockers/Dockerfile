FROM alpine:latest

# Install basic dependencies
RUN apk add --no-cache libc6-compat build-base wget tar iperf3 bash


# Install Go
RUN wget https://go.dev/dl/go1.19.1.linux-amd64.tar.gz && \
    rm -rf /usr/local/go && \
    tar -C /usr/local -xzf go1.19.1.linux-amd64.tar.gz && \
    rm -f go1.19.1.linux-amd64.tar.gz

ENV PATH="$PATH:/usr/local/go/bin"

# Install python
ENV PYTHONUNBUFFERED=1

RUN apk add --no-cache python3 py3-pip && \
    python3 -m venv /opt/venv && \
    /opt/venv/bin/pip install --upgrade pip setuptools wheel

# Add venv to PATH so python and pip use it by default
# ENV PATH="/opt/venv/bin:$PATH"


# CMD ["python3"]
