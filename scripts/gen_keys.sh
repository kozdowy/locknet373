#!/bin/bash
FILE=$1
if [ -z $FILE ] 
then
	echo "Usage: ./gen_keys.sh <FILE_NAME>"
	exit 0
fi
touch ${FILE}.pem
openssl genrsa -out ${FILE}.pem -F4 64
openssl rsa -in ${FILE}.pem -pubout > ${FILE}.pub
openssl rsa -pubin -inform PEM -text -noout < ${FILE}.pub > ${FILE}.pub.txt
openssl rsa -in ${FILE}.pem -text -noout > ${FILE}.pem.txt
exit 1
