 #!/bin/bash
 if [ "$#" -eq  "1" ]
   then
     echo "Compiling example $1."
     cd ./examples
    if   [ "$1" -ge "1" ] && [ "$1" -le "9" ]
        then
        cd ./EX0$1
        ./EX0$1.sh
        mv out.html ../../
    elif [ "$1" -ge "10" ] && [ "$1" -le "15" ]
        then 
            cd ./EX$1
            ./EX$1.sh
    else
        echo "The example number must be between 1 and 15."
    fi
 else
     echo "The number of example to compile must be provided. \n"
 fi
