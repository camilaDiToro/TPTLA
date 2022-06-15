for i in {1..10}; do curl -w "\n" https://icanhazdadjoke.com  >> jokes ; done; cat jokes | ../../jtoh EX06-dad-jokes.json TLA 5
rm jokes