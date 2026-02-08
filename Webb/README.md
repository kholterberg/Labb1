# Labb 1 - Del 1: Webbserver

Detta är en webbserver skriven i C som använder Berkeley Sockets (TCP) för att leverera statiska filer till en webbläsare.

## Funktioner
* Hanterar HTTP GET-förfrågningar.
* Serverar filer dynamiskt från mappen `sample_website`.
* Identifierar automatiskt filtyp (MIME-type) för:
  * HTML (.html)
  * JPEG (.jpg, .jpeg)
* Skickar 'HTTP 200 OK' om filen hittas.
* Skickar 'HTTP 404 Not Found' om filen saknas.

## Kompilering
Kompilera koden med GCC via terminalen:

gcc server.c -o server
