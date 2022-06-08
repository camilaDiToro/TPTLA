# TPTLA
## Construcción y ejecucción
Para construir el proyecto, realizar un make en el directorio principal
```console
foo@bar:~/TPTLA/$ make all
```
Luego encontraremos el ejecutable del compilador **jtoh** en el directorio principal. Los codigos fuente de los programas de ejemplo se encuentran en el 
directorio *examples* mientras que los codigos fuente para los programas de ejemplo de error estan en *./examples/error_cases*. El compilador lleva como argumento el archivo fuente. Ejemplo de compilacion: 
```console
foo@bar:~/TLA_TPE/$ ./jtoh EX01-json-a-html-estandar.json
```
De forma alternativa, se puede correr cualquiera de los ejemplos de la siguiente forma:
```console
foo@bar:~/TLA_TPE/$ ./run.sh i
```
Donde i es un entero entre 1 y 10, y se corresponde con el numero de ejemplo. Los ejemplos 11 a 15 representa ejemplos con errores.