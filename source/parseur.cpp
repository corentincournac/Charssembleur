#include "parseur.h"
#include <ctime>
#include <stdlib.h>

std::vector<std::string> getFile(std::string filename)
{
    std::vector<std::string> tab;
    std::ifstream file(filename.c_str());
    //file.open(filename, std::fstream::in);
    while (!file.eof())
    {
        std::string str;
        getline(file, str);
        //file >> str;
        if (!str.empty())   //si la chaine est vide on en veut pas
        {
            if (str[0]!=';')    //si c'est un full commentaire non plus
                tab.push_back(str);
        }

    }

    for (int i=0; i<tab.size(); i++)
    {
        for (int j=0; j<tab[i].size(); j++)
        {
            //std::cout<<i<<" "<<j<<"\n";

            if (tab[i][j]==';')
            {
                tab[i]=tab[i].substr(0,j);
                break;
            }

            if (tab[i][j]=='$')  //ce qui est entre 2 $ est un chiffre. $$ correspond au symbole '$'
            {
                //std::cout<<"entering $";
                int k=j+1;
                bool r(false);
                char val(0);
                if (tab[i][k]!='$')
                {
                    val=val*10+tab[i][k]-'0';
                    k++;
                    r=true;
                }
                if (tab[i][k]!='$')
                {
                    val=val*10+tab[i][k]-'0';
                    k++;
                }
                if (tab[i][k]!='$')
                {
                    val=val*10+tab[i][k]-'0';
                    k++;
                }
                if (tab[i][k]!='$')
                {
                    std::cout<<"Erreur : les nombres entre '$' doivent etre entre 0 et 255 !";
                    std::cout<<"\nComme ce parseur n'est pas prévu pour planter, on plante pas !";
                    std::cout<<"\nBugs à prévoir !";
                }

                if (!r)
                    val = '$';

                tab[i]=(tab[i].substr(0,j) +val)+ tab[i].substr(k+1,tab[i].size()-1) + ";";

                //std::cout<<tab[i]<<std::endl;
                //std::cout<<"lol";
            }
        }
        ///------------décommenter pour que ça affiche le programme avant son exécution.
        //std::cout << tab[i] << std::endl;
    }

    return tab;

}

void next(std::vector<std::string>& data, int& x, int& y)
{
    x++;
    if (x==data[y].size())
    {
        x=0;
        y++;
        if (y==data.size())
        {
            y=0;
        }
    }
}

int parse(std::vector<std::string>& data)
{
    int instructionx(0), instructiony(0), datax(0), datay(0);
    bool flag(false);
    std::vector<char> pile;
    std::vector<int> datastorex;
    std::vector<int> datastorey;
    std::vector<int> callx, cally;

    srand(time(0));

    //std::cout<<data.size();

    while (data[instructiony][instructionx] != 'q')  //boucle principale
    {
        if (instructiony<0 || instructiony>=data.size())
        {
            std::cout<<"\nErreur : instruction y hors limites.\n";
            return 1;
        }
        if (instructionx<0 || instructionx>=data[instructiony].size())
        {
            std::cout<<"\nErreur : instruction x hors limites.\n";
            return 1;
        }
        if (datay<0 || datay>=data.size())
        {
            std::cout<<"\nErreur : data y hors limites.\n";
            return 1;
        }
        if (datax<0 || datax>=data[datay].size())
        {
            std::cout<<"\nErreur : data x hors limites.\n";
            return 1;
        }

        char c = data[instructiony][instructionx];

        ///---------------------------------------
        //std::cout<<c;
        ///---------------------------------------

        if (c=='#') //etiquette
        {
            next(data,instructionx,instructiony);   //on saute le #
            //next(data,instructionx,instructiony);   //et la valeur de l'étiquette
        }
        else if (c=='j' || c=='k')  //saut | saut conditionnel
        {
            next(data, instructionx, instructiony); //on va sur la case qui suit;
            char etiquette =  data[instructiony][instructionx]; //on lit la valeur de l'étiquette
            if (c=='j' || (c=='k' && flag))
                if (!findEtiquette(data, etiquette, instructionx, instructiony)) //et on y saute
                {
                    std::cout<<"\nErreur : "<< data[instructiony] <<" : Etiquette '" <<etiquette<<"' non existante.\n";
                    return 1;
                }
        }
        else if (c=='s')    //set de data à l'étiquette
        {
            next(data, instructionx, instructiony); //on va sur la case qui suit;
            char etiquette =  data[instructiony][instructionx]; //on lit la valeur de l'étiquette
            //std::cout<<"etiq :" <<etiquette<<"\n";
            if (!findEtiquette(data, etiquette, datax, datay)) //et on y saute
            {
                std::cout<<"\nErreur : "<< data[instructiony] <<" :  Etiquette '" <<etiquette<<"' non existante.\n";
                return 1;
            }

            next(data,datax,datay); //comme c'est le data, il saute pas tout seul : on l'aide !

        }
        else if (c=='q')    //quit
        {
            return 0;
        }
        else if (c=='a')    //afficher
        {
            //std::cout<<"datax:"<<datax<<" datay:"<<datay<<std::endl;
            std::cout<<data[datay][datax];
        }
        else if (c=='d')    //data->droite
        {
            next(data,datax,datay);
        }
        else if (c=='g')    //data->gauche
        {
            prev(data,datax,datay);
        }
        else if (c=='h')    //data->haut
        {
            up(data,datax,datay);
        }
        else if (c=='b')    //data->haut
        {
            down(data,datax,datay);
        }
        else if (c=='c')    //compare
        {
            next(data, instructionx, instructiony); //on va sur la case qui suit;
            char val =  data[instructiony][instructionx]; //on lit la valeur de l'étiquette

            if (data[datay][datax]==val)
                flag=true;
            else
                flag=false;
        }
        else if (c=='i')    //input. Ecrit l'entrée standard là où est data.
        {
            char val;
            std::cin>>val;
            data[datay][datax]=val;
        }
        else if (c=='l')    //push de data dans la pile
        {
            pile.push_back(data[datay][datax]);
        }
        else if (c=='e')    //pop de la pile vers data
        {
            if (pile.empty())
            {
                std::cout <<"\nErreur : la pile est vide.\n";
                return 1;
            }
            char r = pile.back();
            pile.pop_back();
            data[datay][datax]=r;
        }
        else if (c=='+')    //+1
        {
            if (data[datay][datax]==255)
            {
                data[datay][datax]=0;
                flag=1;
            }
            else
            {
                data[datay][datax]++;
                flag=0;
            }
        }
        else if (c=='-')    //-1
        {
            if (data[datay][datax]==0)
            {
                data[datay][datax]=255;
                flag=1;
            }
            else
            {
                data[datay][datax]--;
                flag=0;
            }
        }
        else if (c=='f')    //fonction. comme jump mais retourne à la valeur initiale si trouve 'r'
        {
            next(data, instructionx, instructiony); //on va sur la case qui suit;
            callx.push_back(instructionx);  //après le next, sinon ça tentera d'exécuter ce qui suit !
            cally.push_back(instructiony);
            char etiquette =  data[instructiony][instructionx]; //on lit la valeur de l'étiquette

            if (!findEtiquette(data, etiquette, instructionx, instructiony)) //et on y saute
            {
                std::cout<<"\nErreur : "<<data[instructiony] <<" : Etiquette '" <<etiquette<<"' non existante.\n";
                return 1;
            }
        }
        else if (c=='r')
        {
            if (callx.empty())
            {
                std::cout<<"\nErreur : 'r' unexpected\n";
                return 1;
            }
            instructionx=callx.back();
            instructiony=cally.back();
            callx.pop_back();
            cally.pop_back();
        }
        else if (c=='?')
        {
            data[datay][datax]=rand()%256;
        }
        else if (c=='m')    //sauver la position de data
        {
            datastorex.push_back(datax);
            datastorey.push_back(datay);
        }
        else if (c=='v')    //aller à la dernière position de data
        {
            if (datastorex.empty())
            {
                std::cout << "\nErreur : la pile DATA est vide\n";
                return 1;
            }

            datax = datastorex.back();
            datay = datastorey.back();
            datastorex.pop_back();
            datastorey.pop_back();
        }
        else if (c=='w') //write met une valeur immédiate à l'endroit où se trouve data
        {
            next(data,instructionx,instructiony);
            char val = data[instructiony][instructionx];
            data[datay][datax]=val;
        }
        else if (c=='!') //not : inverse flag
        {
            flag = !flag;
        }
        else
        {
            std::cout<<std::endl<<"Erreur : instruction '"<<c<<"'  ("<<(int)c<<") non reconnue"<<std::endl;
            return -1;
        }



        next(data,instructionx, instructiony);
    }

    return 0;
}

bool findEtiquette(std::vector<std::string>& tab, char etiquette, int& x, int& y)
{
    for (int i=0; i<tab.size(); i++)
    {
        for (int j=0; j<tab[i].size(); j++)
        {
            //std::cout<<i<<" "<<j<<":\n";
            if (tab[i][j]=='#')
            {
                x=j;
                y=i;
                //std::cout<<sx<<" "<<sy<<" " << tab[sy][sx] <<"\n";
                next(tab,x,y);

                //std::cout<<sx<<" "<<sy<<" " << tab[sy][sx] <<"\n";

                if (tab[y][x]==etiquette)
                {
                    return true;
                }

                //std::cout<<tab[i][j];
                /*std::cout<<i<<" "<<j<<"\n";
                std::cout<<tab[i][j];*/

            }
        }
    }

    return false;

}

void prev(std::vector<std::string>& tab, int& x, int& y)   //décalage de 1 à gauche
{
    x--;
    if (x==-1)
    {
        y--;
        if (y==-1)
            y=tab.size()-1;
        x=tab[y].size()-1;
    }
}

void up(std::vector<std::string>& tab, int& x, int& y)
{
    y--;
    if (y==-1)
        y=tab.size()-1;
    if (x>tab[y].size()-1)
        x=tab[y].size()-1;
}

void down(std::vector<std::string>& tab, int& x, int& y)
{
    y++;
    if (y==tab.size())
        y=tab.size()-1;
    if (x>tab[y].size()-1)
        x=tab[y].size()-1;

}

