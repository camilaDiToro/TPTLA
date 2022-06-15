# TPTLA
## Construcción y ejecucción
Para construir el proyecto, se debe realizar un make en el directorio principal
```console
foo@bar:~/TPTLA/$ make all
```
El ejecutable del compilador **jtoh** se encuentra en el directorio principal. Los codigos fuente de los programas de ejemplo se encuentran en el 
directorio *examples* mientras que los codigos fuente para los programas de ejemplo de error están en *./examples/error_cases*. El compilador lleva como argumento el archivo fuente. Ejemplo de compilación: 
```console
foo@bar:~/TLA_TPE/$ ./jtoh ./examples/EX01/EX01-json-a-html-estandar.json
```
De forma alternativa, se puede correr cualquiera de los ejemplos de la siguiente forma:
```console
foo@bar:~/TLA_TPE/$ ./run.sh i
```
Donde i es un entero entre 1 y 18, y se corresponde con el numero de ejemplo. Los ejemplos 11 a 18 representan ejemplos con errores.

En cualquier caso, se podrá ver la salida HTML en el archivo out.html, en en directorio principal. 