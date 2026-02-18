# Labb 1 - Del 2: Tidsserver

Detta är en tidsserver och en klient skriven i C som använder Berkeley Sockets (UDP) efter standarden RFC 868.

## Funktioner
* Använder UDP, User Datagram Protocol, SOCK_DGRAM, för snabb och anslutningslös kommunikation.
* Använder tidsprotokollet RFC 868 som svarar med tiden i sekunder sedan jan 1 1900.
* Beräknar tidsskillnaden melllan RFC 868 och Unix tid som började jan 1 1970.
* Ser till att skickad data är i big endian med hjälp av htonl, Host To Network Long, och att när klienten tar emot datan ordnas det efter klienten med hjälp av ntohl, Network To Host Long.
* Med RFC 868 skickas tiden i 32 bits format, för att vara förståeligt för en mänsch används funktionen ctime för att översätta det till ett mer läsligt datum.

## Kompilering
Kompilera koden med GCC via terminalen:

gcc tid_server.c -o tid_server
gcc tid_client.c -o tid_client