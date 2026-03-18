# Setup certificates


# Setup CA certificate

The CA certificate will self-signed the server certificate and clients certificates. This allow the possibility to have client & server certificates with a better certificate management, by updating only the CA when it expire. 

<br>

**1. Generate the CA key and the certificate**
```bash
openssl genrsa -out ca.key 2048

openssl req -new -x509 -days 3650 -key ca.key -out ca.crt -subj "/CN=MyMQTT-CA"
```
<br>

**2. Create Openssl config**

Generate a openssl config to add a SAN to the CN ip of certificate. We can add many ip as possible, it is usefull to have a config adapted for mulltiple networks. 

For that create *openssl.cnf*:

```yml
[req]
default_bits = 2048
prompt = no
default_md = sha256
req_extensions = req_ext
distinguished_name = dn

[dn]
CN = localhost

[req_ext]
subjectAltName = @alt_names

[alt_names]
IP.1 = 192.168.0.25 # ip réseau CESI IoT 
IP.2 = 172.20.10.3 # ip réseau partage de co
IP.3 = 127.0.0.1
DNS.1 = localhost
```

After that we can generate the server certificate to have the SAN. This one is mandatory for openssl connection. 
SAN is the system used by latest openssl versions, the CN isn't use to verify the certificate. 

<br>

---------------------
<br>
<br>
<br>

# Setup Server certificate

<br>

**1. Generate server key**
```bash
openssl genrsa -out server.key 2048
```
<br>

**2. Generate server certificate**
```bash
openssl req -new -key server.key -out server.csr -subj "/CN=localhost"
```
<br>

**3. Sign it with CA and use openssl config**
```bash
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt -days 3650 -extensions req_ext -extfile openssl.cnf
```
<br>

------------------------
<br>
<br>
<br>
<br>

# Restart docker compose 

You can restart the docker compose by the following commands: 
```bash
docker compose down
docker compose up -d
```