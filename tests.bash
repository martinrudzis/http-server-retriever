#!/bin/bash

# Retriever accessing real web server
# retriever [targethost] [path] [port]
./retriever "www.baidu.com" "/" "80"

# Retriever accessing a file from my server
./retriever "csslab12h.uwb.edu" "file1.txt" "1041"

# Retriever requesting an unauthorized file from my server
./retriever "csslab12h.uwb.edu" "SecretFile.html" "1041"

# Retriever requesting a forbidden file from my server
./retriever "csslab12h.uwb.edu" "./../../etc/passwd" "1041"

# Retriever requesting a non-existent file from my server 
./retriever "csslab12h.uwb.edu" "nonexistantfile.html" "1041"

# Retriever sending a malformed request to my server 

# Server tests 

