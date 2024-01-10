#!/bin/sh

echo "Building app"
npm run build

echo "Deploying files to server"
rsync -avP dist/ root@198.58.113.142:/var/www/golem.lol/
echo "Deployment complete"
