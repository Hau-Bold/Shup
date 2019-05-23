    Autor: Michael Krasser
    Shell und Prozesse SS 2019, IF8
    Blatt2: Prozesse unter Linux
    Aufgabe 22

    Vorbedingungen:
    Das aktuelle Arbeitsverzeichnis ist blatt2/a25

    Generierung:
    make
    (Makefile liegt im aktuellen Arbeitsverzeichnis)

    Bedienung:
    ./a22 argumente
    Beispiel:
    ./a22 eins zwei drei vier
    Ausgabe (ausgehend von Beispiel):

parent: 2705 parameter:eins
son: 2706 from father 2705, parameter:zwei
son: 2707 from father 2705, parameter:drei
son: 2708 from father 2705, parameter:vier
Waiting one second!
I have finished!
terminate <15> first son with pid 2706
terminate <9> second son with pid 2707
terminate <9> third son with pid 2708
son with pid 2708 exited with status 2
son with pid 2707 ended because of signal: 9
son with pid 2706 ended because of signal: 15

