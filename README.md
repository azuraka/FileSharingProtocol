# File Sharing Protocol
<br />
Read Assignment2.pdf for problem statement
<br />
To run:<br />
1) ./run.sh<br />
2) ./server-client1<br />
3) open new terminal and run ./server-client2<br /><br />

Both of the programs running act as server and client (used fork() --> one parent connected with other child and vice-versa).<br />
One can upload a file in a shared folder<br />
Other can search for a file and download that file<br />
(TCP/UDP)<br />
Basic index search and file upload is done. Other features can be inplemented by extending the code in similar fashion.<br /><br />

/* For FileUpload Algorithm<br /><br />

Client (gives filename as command, parse it):<br /><br />

(i)       Send File name to Server in a variable using the function send()<br />
(ii)      take fp = (filename,read)<br />
(iii)     use fread to read the file<br />
(iv)      read small chunks and put them in a buffer (char variable)<br />
(v)       send those to server<br /><br />

Server:<br /><br />

(i)       Make a newfile using fopen in write mode in shared folder and giving it file name that was being sent by client<br />
(ii)      write the file with the chunk of data being sent (in buffer variable)<br />
(iii)     Send an acknowledgement message to client regarding this<br />
