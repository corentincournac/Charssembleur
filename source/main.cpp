#include <iostream>
#include "parseur.h"

int main(int argc, char* argv[])
{
    std::string filename;
    if (argc>1)
    {
        filename=argv[1];
        //std::cout<<filename;
    }
    else
    {
        std::cout<<"Le plus simple est d'entrer le nom du fichier contenant le programme en paramètre à l'exécution.\n";
        std::cout<<"En gros faire 'charsembleur prog'\nCeci dit je vous en veux pas, j'ai tout prévu !\nEntrez ici le nom du programme à interpréter -> ";
        std::cin >>filename;
        std::cout<<"---"<<std::endl;
    }

    //std::cout<<"Yoloyoloyolo"<<std::endl;

    /*std::cout<<"Le programme :" <<std::endl;*/
    std::vector<std::string> tab = getFile(filename);

    //std::cout<<(int)'\n';

    /*std::cout<<"\n\n\n";
    //std::cout<<tab.size();
    std::cout<<"Debut directement sous cette ligne :";
    //


    std::cout<<std::endl;*/


    int i = parse(tab);

    std::cout << std::endl<<std::endl;
    if (i==0)
        std::cout<<"---Le programme a terminé normalement.---\n";
    else
        std::cout<<"---Le programme a terminé avec des erreurs.---\n";

    std::cout<<"Veuillez entrer n'importe quelle chaîne pour quitter -> ";
    std::cin >> filename;

    return 0;
}
