#!/bin/sh

echo "Building app"
npm run build

echo "Deploying files to server"
rm -r /var/www/golem.lol/*
cp -r dist/* /var/www/golem.lol/
echo "Deployment complete"
