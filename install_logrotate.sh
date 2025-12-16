#!/usr/bin/env bash
set -e

APP_NAME=${1}

sudo mkdir -p /etc/rsyslog.d /etc/logrotate.d

sed "s|@@APP_NAME@@|${APP_NAME}|g" ./rsyslog.conf.in \
    | sudo tee /etc/rsyslog.d/${APP_NAME}.conf > /dev/null

#sed "s|@@APP_NAME@@|${APP_NAME}|g" ./logrotate.conf.in \
#    | sudo tee /etc/logrotate.d/${APP_NAME} > /dev/null

sudo systemctl restart rsyslog
