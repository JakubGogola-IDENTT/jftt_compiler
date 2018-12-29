Projekt kompilatora w ramach kursu <b>Języki Formalne i Techniki Translacji</b>. Przedmiot realizowany dla studentów informatyki WPPT Politechniki Wrocławskiej w semestrze zimowym 2018/2019.

Aby skompilowac projekt należy użyć polecania `make`. W celu kompilacji kodu należy użyć polecenia `./compiler filename.imp`, gdzie `filename.imp` jest nazwą pliku zawierającą kod źródłowy. Kod wynikowy, w przypadku bezbłędnej kompilacji, zostanie wypisany do pliku `filename.out`. W celu uruchomienia skompilowanego programu należy użyć maszyny rejestrowej znajdującej się w folderze <i>maszyna-rejestrowa</i>.

Kompilator został przetestowany i skompilowany pod systemem `Kubuntu 18.10 (Cosmic Cuttlefish)` dla kompilatora języka `C++` `g++` dla standardu `c++11` (wiąże się to z pewnymi ogarniczeniami pozostałych dwóch, dalej wymienionych programów), dla programu `flex` w wersji 2.6.4 i dla programu `bison` w wersji 3.0.4.

TODO: Specyfikacja języka.
