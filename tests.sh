#!/bin/bash

# Retriever accessing real web server
# retriever [targethost] [path] [port]
./retriever "www.baidu.com" "/" "80" &> test2result.txt
if grep -q "HTTP/1.1 200 OK" "test2result.txt" ; then 
   echo "test case 2: pass"
else
   echo "test case 2: fail"
   echo "result was: '$firstline'"
fi

# Retriever accessing a file from my server
./retriever "csslab12h.uwb.edu" "file1.txt" "1041" &> test3result.txt
if grep -q "HTTP/1.1 200 OK" "test3result.txt" ; then 
   echo "test case 3: pass"
else
   echo "test case 3: fail"
   echo "result was: '$firstline'"
fi

# Retriever requesting an unauthorized file from my server
./retriever "csslab12h.uwb.edu" "SecretFile.html" "1041" &> test4result.txt
if grep -q "HTTP/1.1 401 Unauthorized" "test4result.txt" ; then 
   echo "test case 4: pass"
else
   echo "test case 4: fail"
   echo "result was: '$firstline'"
fi

# Retriever requesting a forbidden file from my server
./retriever "csslab12h.uwb.edu" "../../../etc/passwd" "1041" &> test5result.txt
if grep -q "HTTP/1.1 403 Forbidden" "test5result.txt" ; then 
   echo "test case 5: pass"
else
   echo "test case 5: fail"
   echo "result was: '$firstline'"
fi

# Retriever requesting a non-existent file from my server 
./retriever "csslab12h.uwb.edu" "nonexistantfile.html" "1041" &> test6result.txt
if grep -q "HTTP/1.1 404 Not Found" "test6result.txt" ; then 
   echo "test case 6: pass"
else
   echo "test case 6: fail"
   echo "result was: '$firstline'"
fi

# Retriever sending a malformed request to my server 
./retriever "csslab12h.uwb.edu" "HTTP/1.1" "1041" &> test7result.txt
if grep -q "HTTP/1.1 400 Bad Request" "test7result.txt" ; then 
   echo "test case 7: pass"
else
   echo "test case 7: fail"
   echo "result was: '$firstline'"
fi
