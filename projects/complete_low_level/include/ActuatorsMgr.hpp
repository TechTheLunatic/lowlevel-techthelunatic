#ifndef ACTUATORSMGR_HPP
#define ACTUATORSMGR_HPP

#include <ax12.hpp>
#include <Uart.hpp>
#include <Singleton.hpp>
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

extern Uart<1> serial;

//Vitesses AX12
//Pelle
#define vPelleDeb 25
#define vPelleMid 20
#define vPelleFin 45

//Bras Pelle
#define vBraPelRel 20
#define vBraPelMed 15
#define vBraPelDep 10

#define vCaleDesc 45
#define vCaleUp 100


//Positions des AX12

//Pour la Pelleteusatron 3000

#define brapeldepD 105
#define brapeldepmibasD 123
#define brapelramD 91
#define brapelmoitD 148
#define brapelrelD 178

#define brapeldepG 121
#define brapeldepmibasG 139
#define brapelramG 107
#define brapelmoitG 164
#define brapelrelG 194

#define pospelinitG 300
#define pospeldeplG 180
#define pospelmoitG 155

#define pospel1G 245
#define pospel1D 55
#define pospel2G 260
#define pospel2D 40

#define pospeltientG 121
#define pospeldeliG 0
#define pospelinitD 0
#define pospeldeplD 120
#define pospelmoitD 145
#define pospeltientD 179
#define pospeldeliD 300

//Pour les attrape-modules

#define AMdebG 180
#define AMmidG 130
#define AMfinG 54
#define AMdebD 115
#define AMmidD 170
#define AMfinD 257 //avant: 232

//Les cale-modules
#define CaleHautG 120
#define CaleReposG 195
#define CaleBasG 235
#define CaleHautD 200
#define CaleReposD 135
#define CaleBasD    90
#define CaleBasDMinus  96

//Largue modules
#define LargueRepos 132
#define LarguePousse 175

/**
 * Classe de gestionnaire d'AX12, comporte toutes les fonctions relatives aux actions effectuées par les ax12 ou groupes d'ax12
 *
 */
class ActuatorsMgr : public Singleton<ActuatorsMgr>
{
private:
    typedef Uart<2> serial_ax;  // On utilise le port série 2 de la stm32
    AX<serial_ax>* ax12test;    // ax12 de test
    AX<serial_ax>* PBD;  //objet gérant les deux AX12 des bras de la pelle
    AX<serial_ax>* PBG;
    AX<serial_ax>* PG;     //ax12 pour la pelle de la pelleteuse
    AX<serial_ax>* PD;

    //AX12 de l'attrape module gauche et droit:
    AX<serial_ax>* AMG;
    AX<serial_ax>* AMD;

    //AX12 pour les calle modules:
    AX<serial_ax>* CMD;//droit
    AX<serial_ax>* CMG;//gauche

    //AX12 pour le largueur
    AX<serial_ax>* LM;

public:
    ActuatorsMgr()
    {
        ax12test = new AX<serial_ax>(0,0,1023); // (ID, Angle_min, Angle_Max)
        ax12test->init();
        PBD = new AX<serial_ax>(1,0,1023); // (ID, Angle_min, Angle_Max)
        PBD->init();
        PBG = new AX<serial_ax>(9,0,1023); // (ID, Angle_min, Angle_Max)
        PBG->init();
        PG = new AX<serial_ax>(2,0,1023);
        PG->init();
        PD = new AX<serial_ax>(8,0,1023);
        PD->init();

        AMG = new AX<serial_ax>(3,0,1023);
        AMG->init();
        AMD = new AX<serial_ax>(4,0,1023);
        AMD->init();

        CMD = new AX<serial_ax>(5,0,1023);
        CMD->init();
        CMG = new AX<serial_ax>(6,0,1023);
        CMG->init();

        LM = new AX<serial_ax>(7,0,1023);
        LM->init();
    }
//suuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuus - xavier
    ~ActuatorsMgr()
    {
        delete(ax12test);
        delete(PBD);
        delete(PBG);
        delete(PG);
        delete(PD);
        delete(AMD);
        delete(AMG);
        delete(CMD);
        delete(CMG);
        delete(LM);
    }

    void setTestId(){ //Permet de regler l'id d'un ax12
        int i;
        serial.printfln("Reglage de l'ID d'un AX12");
        serial.printf("\n");
        serial.printfln("Brancher l'AX12 à régler");
        serial.read(i);
        ax12test->initIDB(0);
        ax12test->init();
        serial.printfln("done");
    }

    void setPelleID(){ //Pour regler l'id des ax12 de la pelle
        int i;

        serial.printfln("Reglage de l'ID des AX12 de la pelle(débrancher tous les ax dont l'id a déjà été set)");
        serial.printfln("Brancher ax12 bras droit de la pelleteuse");
        serial.read(i);
        PBD->initIDB(1);
        PBD->init();

        serial.printfln("Brancher ax12 bras gauche de la pelleteuse");
        serial.read(i);
        PBG->initIDB(9);
        PBG->init();

        serial.printfln("Brancher l'AX12 gauche de la pelle");
        serial.read(i);
        PG->initIDB(2);
        PG->init();

        serial.printfln("Brancher l'AX12 droit de la pelle");
        serial.read(i);
        PD->initIDB(8);
        PD->init();
        serial.printfln("done");
    }

    void setModuleID(){ //Pour regler l'id des ax12 de l'attrappe module
        int i;

        serial.printfln("Reglage de l'ID des AX12 de l'attrappe module(débrancher tous les ax dont l'id a déjà été set)");
        serial.printfln("Brancher l'AX12 droit de l'AM");
        serial.read(i);
        AMD->initIDB(4);
        AMD->init();
        serial.printfln("done");

        serial.printfln("Brancher l'AX12 gauche de l'AM");
        serial.read(i);
        AMG->initIDB(3);
        AMG->init();
        serial.printfln("done");

        serial.printfln("Brancher les AX12 droit des calleurs");
        serial.read(i);
        CMD->initIDB(5);
        CMD->init();
        serial.printfln("done");

        serial.printfln("Brancher les AX12 gauche des calleurs");
        serial.read(i);
        CMG->initIDB(6);
        CMG->init();
        serial.printfln("done");

        serial.printfln("Brancher l'AX12 du largueur");
        serial.read(i);
        LM->initIDB(7);
        LM->init();
        serial.printfln("done");
    }

    void changeangle(uint16_t anglemin,uint16_t anglemax) //permet de modifier les angles max et min de l'ax12 de test
    {
        ax12test->changeAngleMIN((uint16_t )1023*anglemin/300);
        ax12test->changeAngleMAX((uint16_t)1023*anglemax/300);
    }


/*			 ____________________
 * 		   *|                    |*
 *		   *|    Pelle T-3000    |*
 *		   *|____________________|*
 */


    void braPelReleve() //relève les bras de la pelle
    {
        serial.printflnDebug("Leve les bras");
        PBD->changeSpeed(20);
        PBG->changeSpeed(20);
        PBD->goTo(brapelrelD);
        PBG->goTo(brapelrelG);
        serial.printflnDebug("done");
    }

    void braPelDeplie() // déplie les bras de la pelle
    {
        serial.printflnDebug("Baisse les bras");
        PBD->changeSpeed(10);
        PBG->changeSpeed(10);
        PBD->goTo(brapeldepD);
        PBG->goTo(brapeldepG);
        serial.printflnDebug("done");
    }
    void braPelDeplieMiBas() // déplie les bras de la pelle
    {
        serial.printflnDebug("Baisse les bras un peu moins que l'autre ordre");
        PBD->changeSpeed(10);
        PBG->changeSpeed(10);
        PBD->goTo(brapeldepmibasD);
        PBG->goTo(brapeldepmibasG);
        serial.printflnDebug("done");
    }
    void braPelRam() // déplie les bras de la pelle
    {
        serial.printflnDebug("Baisse les bras");
        PBD->changeSpeed(10);
        PBG->changeSpeed(10);
        PBD->goTo(brapelramD);
        PBG->goTo(brapelramG);
        serial.printflnDebug("done");
    }

    void braPelMoit()
    {
        serial.printflnDebug("Leve les bras mais pas trop");
        PBD->changeSpeed(20);
        PBG->changeSpeed(20);
        PBD->goTo(brapelmoitD);
        PBG->goTo(brapelmoitG);
        serial.printflnDebug("done");
    }

    //la pelle:

    void pellePosDeplacement()
    {
        serial.printflnDebug("Leve au max les bras");
        PG->goTo(pospeldeplG);
        PD->goTo(pospeldeplD);
        serial.printflnDebug("done");
    }

    void pelleInit()
    {
        serial.printflnDebug("Pelle va au début");
        PG->changeSpeed(50);
        PD->changeSpeed(50);
        PG->goTo(pospelinitG);
        PD->goTo(pospelinitD);
        serial.printflnDebug("done");
    }

    void pelleMoit()
    {
        serial.printflnDebug("Pelle tient boules");
        PG->changeSpeed(40);
        PD->changeSpeed(40);
        PG->goTo(pospelmoitG);
        PD->goTo(pospelmoitD);
        serial.printflnDebug("done");
    }

    void pelleTient(){
        serial.printflnDebug("pos de maintient des boules");
        PG->changeSpeed(40);
        PD->changeSpeed(40);
        PG->goTo(pospeltientG);
        PD->goTo(pospeltientD);
        serial.printflnDebug("done");
    }
    void pelleCrat1(){
        serial.printflnDebug("pos 1 de re-recup cratere");
        PG->changeSpeed(20);
        PD->changeSpeed(20);
        PG->goTo(pospel1G);
        PD->goTo(pospel1D);
        serial.printflnDebug("done");
    }
    void pelleCrat2(){
        serial.printflnDebug("pos 2 de re-recup cratere");
        PG->changeSpeed(20);
        PD->changeSpeed(20);
        PG->goTo(pospel2G);
        PD->goTo(pospel2D);
        serial.printflnDebug("done");
    }

    void pelleLib()
    {
        serial.printflnDebug("Pelle jete boules");
        PG->changeSpeed(50);
        PD->changeSpeed(50);
        PG->goTo(pospeldeliG);
        PD->goTo(pospeldeliD);
        serial.printflnDebug("done");
    }

    void pelleReasserv(){
        serial.printflnDebug("Reasservis la pelle");
        PG->unasserv();
        PG->asserv();
        PD->unasserv();
        PD->asserv();
        serial.printflnDebug("done");

    }
/*			 ___________________
 * 		   *|                   |*
 *		   *|  Attrappe Module  |*
 *		   *|___________________|*
 */

    void moduleDebGauche()
    {
        AMG->changeSpeed(40);
        serial.printflnDebug("Attrappe module Gauche position initiale");
        AMG->goTo(AMdebG);
        serial.printflnDebug("done");
    }

    void moduleDebDroite()
    {
        AMD->changeSpeed(40);
        serial.printflnDebug("Attrappe module Droit position initiale");
        AMD->goTo(AMdebD);
        serial.printflnDebug("done");
    }

    void moduleMidGauche()
    {
        AMG->changeSpeed(20);
        serial.printflnDebug("Attrape module gauche position medium");
        AMG->goTo(AMmidG);
        serial.printflnDebug("done");
    }

    void moduleMidDroite()
    {
        AMD->changeSpeed(20);
        serial.printflnDebug("Attrape module droit position medium");
        AMD->goTo(AMmidD);
        serial.printflnDebug("done");
    }



    void moduleFinDroite()
    {
        AMD->changeSpeed(15);
        serial.printflnDebug("Attrape module droit position fin");
        AMD->goTo(AMfinD);  //Si le côté est droit (cote = 0)
        serial.printflnDebug("done");
    }

    void moduleFinGauche()
    {
        AMG->changeSpeed(15);
        serial.printflnDebug("Attrape module gauche position fin");
        AMG->goTo(AMfinG);
        serial.printflnDebug("done");
    }


/*			 ___________________
 * 		   *|                   |*
 *		   *|    Cale Module    |*
 *		   *|___________________|*
 */

    void caleHautDroite(){
        CMD->changeSpeed(vCaleUp);
        serial.printflnDebug("Cale droit remonte");
        CMD->goTo(CaleHautD);
    }
    void caleMidDroite(){
        CMD->changeSpeed(vCaleUp);
        serial.printflnDebug("Cale droit medium");
        CMD->goTo(CaleReposD);
    }
    void caleBasDroite(){
        CMD->changeSpeed(vCaleDesc);
        serial.printflnDebug("Cale droit bas");
        CMD->goTo(CaleBasD);
    }
    void caleBasDMinus(){
        CMD->changeSpeed(vCaleDesc);
        serial.printflnDebug("Cale droit bas mais pas trop");
        CMD->goTo(CaleBasDMinus);
    }

    void caleHautGauche(){
        CMG->changeSpeed(vCaleUp);
        serial.printflnDebug("Cale gauche remonte");
        CMG->goTo(CaleHautG);
    }
    void caleMidGauche(){
        CMG->changeSpeed(vCaleUp);
        serial.printflnDebug("Cale gauche medium");
        CMG->goTo(CaleReposG);
    }
    void caleBasGauche(){
        CMG->changeSpeed(vCaleDesc);
        serial.printflnDebug("Cale gauche bas");
        CMG->goTo(CaleBasG);
    }
/*			 ___________________
 * 		   *|                   |*
 *		   *|   Largue-Module   |*
 *		   *|___________________|*
 */
    void largueRepos(){
        serial.printflnDebug("Initialisation du largue modules");
        LM->changeSpeed(50);
        LM->goTo(LargueRepos);
        serial.printflnDebug("done");

    }
    void larguePousse(){
        serial.printflnDebug("Largue un module");
        LM->changeSpeed(18);
        LM->goTo(LarguePousse);
        serial.printflnDebug("done");
    }
    void larguePousselent(){
        serial.printflnDebug("Largue lentement un module");
        LM->changeSpeed(5);
        LM->goTo(LarguePousse);
        serial.printflnDebug("done");
    }


    // Voilà.



    void setAXpos(uint16_t  position) { // pour d�finir manuellement 1 position
        ax12test->goTo(position);

    }

    void changeAXSpeed(int speed) //fonction inutile faire changeSpeedB(speed)
    {
        ax12test->changeSpeedB(speed);

    }

    bool change_actualpos(uint16_t position)//ne marche pas (permettrait de changer la position de l'ax12 sans le faire bouger)
    {
        return (ax12test->change_actpos(position));
    }

    uint16_t posdeax12()//ne marche pas( permettrait d'obtenir la position des ax12)
    {
        return ax12test->getPositionDegres();
    }

    void reanimation () //réanime les ax12 en changeant leur baudrates
    {
        ax12test->reanimationMode(9600);
    }

    void pelreasserv()
    {
        PD->init();
        PG->init();
        PBD->init();
        PBG->init();
    }

    void setPunch(){
        uint16_t punchL=32;
        uint16_t punchH=0;
        serial.printflnDebug("Entrez punch_H(défaut=0)");
        serial.read(punchH);
        serial.printflnDebug("Entrez punch_L(défaut=32)");
        serial.read(punchL);
        ax12test->setPunch(punchL,punchH);
    }
    void setSlopes(){  //Change la variation de
        uint16_t cwSlope=32;
        uint16_t ccwSlope=32;
        serial.printflnDebug("Entrez CW_SLOPE (défaut=32)");
        serial.read(cwSlope);
        serial.printflnDebug("Entrez CCW_Slope(défaut=32)");
        serial.read(ccwSlope);
        ax12test->setSlopes(cwSlope,ccwSlope);
    }

    void testSync1(){
        serial.printflnDebug("Test sync (");
        PBD->syncGoTo(2,8,180,120);
        serial.printflnDebug("done");
    }

};

#endif /* ACTUATORSMGR_HPP */
