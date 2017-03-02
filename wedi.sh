#IOS Project
#Wrapper - wedi
#Jan Fitz

###################
# Pocatecni testy #
###################

#Testovani dostupnosti utility realpath
if ! [ -x "$(command -v realpath)" ]; then
  echo 'Utilita realpath neni dostupna'
  exit 1
fi

#Testovani existence promenne $WEDI_RC a soubou v ni ulozenem
if [ -z "$WEDI_RC" ] || [ ! -f  "$WEDI_RC" ]; then 
	echo "Neni nastavena promenna" '$WEDI_RC, nebo odkazovany soubor neexistuje'
	exit 1
fi

###################
# Definice funkci #
###################

#Zkontorluje spravnost formatu zadaneho data
check_date_format() {

		echo "$1" | grep -q -E '^([0-9]{4}-[0-9]{2}-[0-9]{2})$'

		if [ $? -ne 0 ]; then
			echo "Nespravny format data"
			exit 1
		fi
}

#Zkontroluje, zda zadany adresar existuje
check_dictonary_existance() {

	if [ ! -d $1 ]; then
		echo "Zadany adresar neexistuje"
		exit 1
	fi
}

#Vypise chyby v pripade neuspechu provadene funkce
if_error() {

	if [ $1 -ne 0 ]; then
		echo  'Vyskytla se chyba se zadanym souborem nebo adresarem'
		exit 1
	fi
}

#Funkce pro modifikaci zadaneho souboru a jeho otevreni v editoru
open_and_modificate_file() {
	
	#Modifikace zadaneho souboru
	touch "$1"

	cesta=(`realpath $1`)
	moddate=$(date +%Y-%m-%d)

	#Zjisteni celeho nazvu souboru pro zapis do referencniho souboru
	soubor=`basename $1`

	#Ulozeni informaci o kontrolovanem souboru do referencniho souboru
	echo $soubor:$cesta:$moddate $'\n'"$(cat "$WEDI_RC")" > "$WEDI_RC"

	#Podminene Otevreni souboru pomoci jednoho z editoru urceneho promennou $EDITOR nebo $VISUAL
	if [ -z "$EDITOR" ] && [ -z "$VISUAL" ] ; then 
		echo "Neni nastaven zadny editor pro modifikaci souboru"
		exit 1
	else 
		$EDITOR "$1" || $VISUAL "$1"
	fi
}

######################
# Parsovani podminek #
######################

#Funkce, ktera vybere posledni editovany soubor z auktualniho adresare a otevre jej k editaci
if [ $# = '0' ]; then

	cesta=`realpath $PWD`

	#Vybere z aktualniho adresare posledni editovany soubor
	file=$(cat "$WEDI_RC" | grep "$cesta" | cut -d ':' -f2 | grep -E "$cesta/[^/]+$" | head -n1 | rev | cut -d '/' -f1 | rev)

	#Test zda nalezeny soubor existuje
	if [ ! -f  "$file" ]; then
		echo "Hledany soubor jiz neexistuje, nebo je poskozen"
		exit 1
	fi

	open_and_modificate_file "$file"
	
	if_error "$?"	

#Funkce, ktera vybere posledni editovany soubor ze zadaneho adresare a otevre jej k editaci
elif [ $# = '1' ] && [ -d $1 ]; then

	cesta=`realpath $1`

	#Vybere z aktualniho adresare posledni editovany soubor
	file=$(cat "$WEDI_RC" | grep "$cesta" | cut -d ':' -f2 | grep -E "$cesta/[^/]+$" | head -n1 | rev | cut -d '/' -f1 | rev)

	#Test zda nalezeny soubor existuje
	if [ ! -f  "$1/$file" ]; then
		echo "Hledany soubor neexistuje, nebo je poskozen"
		exit 1
	fi

	open_and_modificate_file "$1/$file"
	
	if_error "$?"	

#Funkce pro etevreni a editace zadaneho souboru
elif [ $# = '1' ] && [ ! $1 = '-m' ] && [ ! $1 = '-l' ]; then

	open_and_modificate_file "$1"

#Funkce pro editaci nejcasteji editovaneho souboru z aktualniho adresare
elif [ $# = '1' ] && [ $1 = '-m' ]; then

	cesta=`realpath $PWD`

	#Vybere z aktualniho adresare nejcasteji editovany soubor
	file=$(cat "$WEDI_RC" | grep "$cesta" | cut -d ':' -f2 | sort -g | uniq -c | grep -E "$cesta/[^/]+$" | sort -k1,1nr -k2,2 | head -n1 | rev | cut -d '/' -f1 | rev)

	#Test zda nalezeny soubor existuje
	if [ ! -f  "$file" ]; then
		echo "Hledany soubor neexistuje, nebo je poskozen"
		exit 1
	fi

	open_and_modificate_file "$file"
	
	if_error "$?"	

#Funkce pro editaci nejcasteji editovaneho souboru ze zadaneho adresare
elif [ $# = '2' ] && [ $1 = '-m' ]; then

	check_dictonary_existance "$2"

	cesta=`realpath $2`

	#Vybere z aktualniho adresare nejcasteji editovany soubor
	file=$(cat "$WEDI_RC" | grep "$cesta" | cut -d ':' -f2 | sort -g | uniq -c | grep -E "$cesta/[^/]+$" | sort -k1,1nr -k2,2 | head -n1 | rev | cut -d '/' -f1 | rev)

	#Test zda nalezeny soubor existuje
	if [ ! -f  "$2/$file" ]; then
		echo "Hledany soubor neexistuje, nebo je poskozen"
		exit 1
	fi

	open_and_modificate_file "$2/$file"
	
	if_error "$?"	

#Funce pro vypis vsech souboru drive editovanych v aktualnim adresari
elif [ $# = '1' ] && [ $1 = '-l' ]; then

	cesta=`realpath $PWD`

	cat "$WEDI_RC" | grep "$cesta" | cut -d ':' -f2 | sort -g | uniq -c | grep -E "$cesta/[^/]+$" | rev | cut -d '/' -f1 | rev

	if_error $?	

#Funce pro vypis vsech souboru drive editovanych v zadanem adresari
elif [ $# = '2' ] && [ $1 = '-l' ]; then

	check_dictonary_existance "$2"

	cesta=`realpath $2`

	cat "$WEDI_RC" | grep "$cesta" | cut -d ':' -f2 | sort -g | uniq -c | grep -E "$cesta/[^/]+$" | rev | cut -d '/' -f1 | rev

	if_error $?	

#Funkce pro editaci souboru editovaneho pred zadanym datem z aktualniho adreasre
elif [ $# = '2' ] && [ $1 = '-b' ]; then

	check_date_format $2

	cesta=`realpath $PWD`

	cat "$WEDI_RC" | grep "$cesta" | sort -g | uniq -c | grep -E "$cesta/[^/]+$" | sort -k2M -k3 -k4 |rev | cut -d '/' -f1 | rev | while read radek; do
		file=$(echo "$radek" | rev | cut -d ':' -f1 | rev | tr -d "-")
		datum=$(echo "$2" | tr -d "-")

		if [ "$file" -le "$datum" ]; then
			echo "$radek" | cut -d ':' -f1
		fi
	done | sort | uniq 

	if_error $?	

#Funkce pro editaci souboru editovaneho pred zadanym datem ze zadaneho adreasre
elif [ $# = '3' ] && [ $1 = '-b' ]; then

	check_dictonary_existance "$3"

	check_date_format "$2"

	cesta=`realpath $3`

	cat "$WEDI_RC" | grep "$cesta" | sort -g | uniq -c | grep -E "$cesta/[^/]+$" | sort -k2M -k3 -k4 |rev | cut -d '/' -f1 | rev | while read radek; do
		file=$(echo "$radek" | rev | cut -d ':' -f1 | rev | tr -d "-")
		datum=$(echo "$2" | tr -d "-")

		if [ "$file" -le "$datum" ]; then
			echo "$radek" | cut -d ':' -f1
		fi
	done | sort | uniq 

	if_error "$?"	

#Funkce pro editaci souboru editovaneho po zadanem datu z aktualniho adreasre
elif [ $# = '2' ] && [ $1 = '-a' ]; then

	check_date_format "$2"

	cesta=`realpath $PWD`

	cat "$WEDI_RC" | grep "$cesta" | sort -g | uniq -c | grep -E "$cesta/[^/]+$" | sort -k2M -k3 -k4 |rev | cut -d '/' -f1 | rev | while read radek; do
				
		#Oreze radek po prvni dvojtecku a vymaze z nej pomlcky
		file=$(echo "$radek" | rev | cut -d ':' -f1 | rev | tr -d "-")
		datum=$(echo "$2" | tr -d "-")

		#Porovna zadane datum s tadem na kazdem radku
		if [ "$file" -ge "$datum" ]; then
			echo "$radek" | cut -d ':' -f1
		fi
	done | sort | uniq 

	if_error "$?"	

#Funkce pro editaci souboru editovaneho po zadanem datu ze zadaneho adresare
elif [ $# = '3' ] && [ $1 = '-a' ]; then

	check_dictonary_existance "$3"

	check_date_format "$2"

	cesta=`realpath $3`

	cat "$WEDI_RC" | grep "$cesta" | sort -g | uniq -c | grep -E "$cesta/[^/]+$" | sort -k2M -k3 -k4 |rev | cut -d '/' -f1 | rev | while read radek; do
		file=$(echo "$radek" | rev | cut -d ':' -f1 | rev | tr -d "-")
		datum=$(echo "$2" | tr -d "-")

		if [ "$file" -ge "$datum" ]; then
			echo "$radek" | cut -d ':' -f1
		fi
	done | sort | uniq 

	if_error "$?"	

#Osetreni vsech nedefinovych stavu, ktere za behu programu nastanou
else
	echo 'Argumenty byly zadany ve spatnem formatu, nebo jsou problemy se souborem'
	exit 1
fi

exit 0
