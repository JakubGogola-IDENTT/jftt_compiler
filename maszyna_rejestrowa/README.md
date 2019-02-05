Maszyna rejestrowa stworzona na potrzeby implementacji kompilatora na semestr zimowy 2018 dla studentów informatyki WPPT Politechniki Wrocławskiej. Programy zostały napisane przez <b>dra Macieja Gębalę</b>.

Aby skompilować program należy użyć polecenia `make`. Aby uruchomić program nalezy użyć jedneo z dwóch poleceń:

1. `./maszyna-rejestrowa filename.mr` (wersja obłusgująca liczby typu <i>long long</i>)
2. `./maszyna-rejestrowa-cln filename.mr` (wersja obsługjąca liczby dowolnej długości, korzystająca z biblioteki `CLN` dla języka `C++`)

Maszyna została przetestowana i skompilowana pod systemem `Ubuntu 18.10 (Cosmic Cuttlefish)` dla kompilatora języka `C++` `g++` dla standardu `c++17`, dla programu `flex` w wersji 2.6.4 i dla programu `bison` w wersji 3.0.4.
