#ifndef PAR
#define PAR

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> getFile(std::string name); //renvoie un tableau de string : c'est notre environnement en 2D.

int parse(std::vector<std::string>& data);    //le main

void next(std::vector<std::string>& data, int& x, int& y);  //décale à la case suivante, avec gestion du retour à la ligne.
void prev(std::vector<std::string>& data, int& x, int& y);
void up(std::vector<std::string>& data, int& x, int& y);
void down(std::vector<std::string>& data, int& x, int& y);

bool findEtiquette(std::vector<std::string>& data, char etiquette, int& x, int& y); //fait pointer x et y sur l'étiquette.



#endif
