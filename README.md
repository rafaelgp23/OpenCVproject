Gustavo Lopes 20961
Rafael Guimarães de Paula 21156

GitHub: https://github.com/rafaelgp23/BackUpBCrypt

----------------------------------------------backUp usando BCrypt----------------------------------------------

Primeiramente, o programa precisa da biblioteca Botan instalada para executar. Execute o script "install-botan.sh" para que ele possa funcionar.

Para isso realize os seguintes comandos no terminal do Linux:

$ chmod +x install-botan.sh

$ sudo ./install-botan.sh

A senha está definida como "maurilio", e é salva dentro do código através de um hash gerado pelo algoritmo bcrypt.

A interface do programa é bastante intuitiva. Abra um arquivo .txt e ele será mostrado na caixa de texto do aplicativo.

Clicando em encriptar, um arquivo .txt.bfe será gerado e salvo no diretório selecionado previamente. O .txt antigo será deletado caso a encriptação tenha sucesso.

A desencriptação realiza o processo em todos os arquivos .txt.bfe presentes no diretório selecionado, gerando novamente um arquivo .txt.

Para toda encriptação e desencriptação, o programa requer que o usuário entre com a mesma senha do início do programa. A chave utilizada para encriptação é a própria senha.

Caso o usuário erre a senha 3 vezes seguidas, o aplicativo será encerrado.
