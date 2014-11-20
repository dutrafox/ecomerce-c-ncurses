### Para o correto funcionamento e necessario estar usando o termianl como root e dar as permissoes para o arquivo instalarNCURSES.sh
#Descompactar a NCURSES
echo "### Descompactando ###"
tar -vzxf ncurses-5.9.tar.gz
#Entrar no diretório
cd ncurses-5.9
#Configurar padrao
echo "### Configurando ###"
./configure
#Compilar
echo "### Compilando ###"
make
#Instalar
echo "### Instalando ###"
make install
