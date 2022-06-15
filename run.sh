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
    elif [ "$1" -eq "10" ]
        then 
            cd ./EX$1
            ./EX$1.sh
            mv out.html ../../
    elif [ "$1" -ge "11" ] && [ "$1" -le "14" ]
        then 
            cd ./EX$1
            ./EX$1.sh
    elif [ "$1" -ge "15" ] && [ "$1" -le "18" ]
        then 
            cd ./EX$1
            ./EX$1.sh
            mv out.html ../../
    else
        echo "The example number must be between 1 and 16."
    fi
 else
     echo "The number of example to compile must be provided. \n"
 fi
