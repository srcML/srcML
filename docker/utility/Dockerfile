FROM alpine:latest

# Install tzdata package for timezone information
RUN apk add --no-cache tzdata bash openssh github-cli

# Set the timezone to Eastern Time (New York)
RUN ln -sf /usr/share/zoneinfo/America/New_York /etc/localtime && \
    echo "America/New_York" > /etc/timezone
