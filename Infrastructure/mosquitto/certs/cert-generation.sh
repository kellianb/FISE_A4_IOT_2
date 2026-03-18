# Generate CA (Certificate Authority) key and certificate
openssl genrsa -out ca.key 2048
openssl req -new -x509 -days 3650 -key ca.key -out ca.crt -subj "/CN=MyMQTT-CA"

# Generate server key
openssl genrsa -out server.key 2048

# Generate server certificate signing request
# IMPORTANT: Replace "192.168.1.100" with your actual broker IP address
openssl req -new -key server.key -out server.csr -subj "/CN=192.168.0.25"

# Sign the server certificate with CA
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt -days 3650 -extensions req_ext -extfile openssl.cnf

# Set proper permissions
chmod 644 ca.crt server.crt
chmod 600 ca.key server.key
