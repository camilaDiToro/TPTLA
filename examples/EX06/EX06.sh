for i in {1..10}; do curl --location --request GET https://pokeapi.co/api/v2/pokemon/$i | jq -r '.name' >> pkmn ; done; cat pkmn | ../../jtoh ./EX06-tabla-pokemon.json
rm pkmn