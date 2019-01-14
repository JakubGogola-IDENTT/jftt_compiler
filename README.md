# Kompilator - JFTT 2018/2019

Projekt kompilatora stworzony w ramach kursu <b>Języki Formalne i Techniki Translacji</b>. Przedmiot realizowany dla studentów III informatyki WPPT Politechniki Wrocławskiej w semestrze zimowym 2018/2019. Wykładowca: <b>dr Maciej Gębala</b>.

## Technologie
Do napisania kompilatora zostały użyte następujące narzędzia:

- <b>Flex</b> w wersji 2.6.4,
- <b>Bison</b> w wersji 3.0.4,
- <b>g++</b> w wersji 8.2.0,
- <b>GNU Make</b> w wersji 4.2.1.

Kompilator został napisany i przetestowany pod systemem `Ubuntu 18.10 (Cosmic Cuttlefish)`. Wykorzystano jezyk `C++` w standardzie <b>`C++17`</b>.

## Kompilacja
W celu kompilacji plików źródłowych należy użyć polecenia `make`.

Aby usunąć pliki powstałe w wyniku kompilacji należy wywołać polecenie `make clean`.

## Uruchamianie kompilatora
Aby uruchomić kompilator należy użyć polecanie `./kompilator filename.imp filename.mr`, gdzie:

- `filename.imp` - nazwa pliku z kodem wejściowym,
- `filename.mr` - nazwa pliku z kodem wynikowym.

Jeżeli kompilacja przebiegnie pomyślnie zostanie zwrócony plik z kodem na maszynę rejestrową. W razie wystąpienia błędów w kodzie wejściowym, zostaną wypisane komunikaty o rodzaju znalezionych błędów oraz miejscu ich występowania. W takim przypadku kompilator nie zwraca pliku z kodem wynikowym.

## Interpreter
Do dyspozycji studentów został oddany interpreter prostego kodu maszynowego autorstwa <b>dra Macieja Gębali</b>. Jest on dostępny do pobrania pod tym [linkiem](https://cs.pwr.edu.pl/gebala/dyd/jftt2018/labor4.zip). Interpreter znajduje się również w niniejszym repozytorium w folderze `maszyna_rejestrowa` wraz z plikiem README opisującym sposób kompilacji i uruchomienia.

## Specyfikacja języka
TODO:




