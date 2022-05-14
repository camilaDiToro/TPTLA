# TPTLA
## Construcción y ejecucción
Para construir el proyecto, realizar un make en el directorio principal
```console
foo@bar:~/TPTLA/$ make all
```
Luego encontraremos el ejecutable del compilador **jtoh** en el directorio principal. Los codigos fuente de los programas de ejemplo se encuentran en el 
directorio *examples* mientras que los codigos fuente para los programas de ejemplo de error estan en *./examples/error_cases*. El compilador lleva como argumento
no obligatorio el archivo fuente (si no se le pasa ningun archivo se leera de STDIN). Ejemplo de compilacion: 
```console
foo@bar:~/TLA_TPE/$ ./jtoh ./examples/exp-example.json
```
