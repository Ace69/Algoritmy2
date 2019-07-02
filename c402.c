
/* c402.c: ********************************************************************}
{* Téma: Nerekurzivní implementace operací nad BVS
**                                     Implementace: Petr Přikryl, prosinec 1994
**                                           Úpravy: Petr Přikryl, listopad 1997
**                                                     Petr Přikryl, květen 1998
**			  	                        Převod do jazyka C: Martin Tuček, srpen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                                 Karel Masařík, říjen 2013
**                                                 Radek Hranický 2014-2018
**
** S využitím dynamického přidělování paměti, implementujte NEREKURZIVNĚ
** následující operace nad binárním vyhledávacím stromem (předpona BT znamená
** Binary Tree a je u identifikátorů uvedena kvůli možné kolizi s ostatními
** příklady):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivní vložení nového uzlu do stromu
**     BTPreorder ...... nerekurzivní průchod typu pre-order
**     BTInorder ....... nerekurzivní průchod typu in-order
**     BTPostorder ..... nerekurzivní průchod typu post-order
**     BTDisposeTree ... zruš všechny uzly stromu
**
** U všech funkcí, které využívají některý z průchodů stromem, implementujte
** pomocnou funkci pro nalezení nejlevějšího uzlu v podstromu.
**
** Přesné definice typů naleznete v souboru c402.h. Uzel stromu je typu tBTNode,
** ukazatel na něj je typu tBTNodePtr. Jeden uzel obsahuje položku int Cont,
** která současně slouží jako užitečný obsah i jako vyhledávací klíč
** a ukazatele na levý a pravý podstrom (LPtr a RPtr).
**
** Příklad slouží zejména k procvičení nerekurzivních zápisů algoritmů
** nad stromy. Než začnete tento příklad řešit, prostudujte si důkladně
** principy převodu rekurzivních algoritmů na nerekurzivní. Programování
** je především inženýrská disciplína, kde opětné objevování Ameriky nemá
** místo. Pokud se Vám zdá, že by něco šlo zapsat optimálněji, promyslete
** si všechny detaily Vašeho řešení. Povšimněte si typického umístění akcí
** pro různé typy průchodů. Zamyslete se nad modifikací řešených algoritmů
** například pro výpočet počtu uzlů stromu, počtu listů stromu, výšky stromu
** nebo pro vytvoření zrcadlového obrazu stromu (pouze popřehazování ukazatelů
** bez vytváření nových uzlů a rušení starých).
**
** Při průchodech stromem použijte ke zpracování uzlu funkci BTWorkOut().
** Pro zjednodušení práce máte předem připraveny zásobníky pro hodnoty typu
** bool a tBTNodePtr. Pomocnou funkci BTWorkOut ani funkce pro práci
** s pomocnými zásobníky neupravujte
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c402.h"
int solved;

void BTWorkOut (tBTNodePtr Ptr)		{
/*   ---------
** Pomocná funkce, kterou budete volat při průchodech stromem pro zpracování
** uzlu určeného ukazatelem Ptr. Tuto funkci neupravujte.
**/

	if (Ptr==NULL)
    printf("Chyba: Funkce BTWorkOut byla volána s NULL argumentem!\n");
  else
    printf("Výpis hodnoty daného uzlu> %d\n",Ptr->Cont);
}

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu tBTNodePtr. Tyto funkce neupravujte.
**/

void SInitP (tStackP *S)
/*   ------
** Inicializace zásobníku.
**/
{
	S->top = 0;
}

void SPushP (tStackP *S, tBTNodePtr ptr)
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
{
                 /* Při implementaci v poli může dojít k přetečení zásobníku. */
  if (S->top==MAXSTACK)
    printf("Chyba: Došlo k přetečení zásobníku s ukazateli!\n");
  else {
		S->top++;
		S->a[S->top]=ptr;
	}
}

tBTNodePtr STopPopP (tStackP *S)
/*         --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
{
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0)  {
		printf("Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
		return(NULL);
	}
	else {
		return (S->a[S->top--]);
	}
}

bool SEmptyP (tStackP *S)
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
{
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu bool. Tyto funkce neupravujte.
*/

void SInitB (tStackB *S) {
/*   ------
** Inicializace zásobníku.
**/

	S->top = 0;
}

void SPushB (tStackB *S,bool val) {
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
                 /* Při implementaci v poli může dojít k přetečení zásobníku. */
	if (S->top==MAXSTACK)
		printf("Chyba: Došlo k přetečení zásobníku pro boolean!\n");
	else {
		S->top++;
		S->a[S->top]=val;
	}
}

bool STopPopB (tStackB *S) {
/*   --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0) {
		printf("Chyba: Došlo k podtečení zásobníku pro boolean!\n");
		return(NULL);
	}
	else {
		return(S->a[S->top--]);
	}
}

bool SEmptyB (tStackB *S) {
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** Následuje jádro domácí úlohy - funkce, které máte implementovat.
*/

void BTInit (tBTNodePtr *RootPtr)	{
/*   ------
** Provede inicializaci binárního vyhledávacího stromu.
**
** Inicializaci smí programátor volat pouze před prvním použitím binárního
** stromu, protože neuvolňuje uzly neprázdného stromu (a ani to dělat nemůže,
** protože před inicializací jsou hodnoty nedefinované, tedy libovolné).
** Ke zrušení binárního stromu slouží procedura BTDisposeTree.
**
** Všimněte si, že zde se poprvé v hlavičce objevuje typ ukazatel na ukazatel,
** proto je třeba při práci s RootPtr použít dereferenční operátor *.
**/

	(*RootPtr)=NULL; //nastavime hodnotu korene na NULL
	  /* V případě řešení smažte tento řádek! */
}

void BTInsert (tBTNodePtr *RootPtr, int Content) {
/*   --------
** Vloží do stromu nový uzel s hodnotou Content.
**
** Z pohledu vkládání chápejte vytvářený strom jako binární vyhledávací strom,
** kde uzly s hodnotou menší než má otec leží v levém podstromu a uzly větší
** leží vpravo. Pokud vkládaný uzel již existuje, neprovádí se nic (daná hodnota
** se ve stromu může vyskytnout nejvýše jednou). Pokud se vytváří nový uzel,
** vzniká vždy jako list stromu. Funkci implementujte nerekurzivně.
**/

	//tBTNodePtr prvek;
	//tBTNodePtr prvek1;
	tBTNodePtr ptr = (*RootPtr);
	if(ptr!=NULL)
	{

	//	tBTNodePtr prvek2;
	tBTNodePtr prvek1;
		while(ptr!=NULL) // projdeme cely strom
		{
			if (ptr->Cont == Content) // obsah je stejny
			{
				return;
			}
			if(ptr->Cont < Content) // pokud je vkladana hodnota vetsi, tak jdeme do leve vetve
			{
				prvek1 = ptr;
				ptr=ptr->RPtr;
			}
			else if(ptr->Cont > Content) // pokud je vkladana hodnota mensi, tak jdeme do prave vetve
			{
				prvek1 = ptr;
				ptr=ptr->LPtr;
			}

		}
		//tBTNodePtr novy = malloc(sizeof(struct tBTNode));
		tBTNodePtr prvek = malloc(sizeof(struct tBTNode));
		if(prvek==NULL) //pokud malloc failnul
		{
			return;
		}
		else
		{
			prvek->RPtr=NULL; // pokud porbehl spravne, inicalizujeme novy prvek na dane hodnoty
			prvek->Cont=Content;
			prvek->LPtr=NULL;
			if(prvek1->Cont > Content) // pokud je hondota noveho prvku vetsi nez Cont
			{
				prvek1->LPtr=prvek; // do leveho uzlu prvku1 vlozime novy
			}
			else
			{
				prvek1->RPtr=prvek; // do praveho uzlu prvku2 vlozime novy
			}
		}
	}

	else // pokud RootPtr==NULL
	{
		tBTNodePtr prvek = malloc(sizeof(struct tBTNode)); // probehne alokace
			if(prvek == NULL)
			{
				return; // pokud se malloc nepovedl, ukonicme
			}
			else
			{
				prvek->LPtr=NULL; // jinak inicializujeme novy prvek na dane hodnoty
				prvek->Cont=Content;
				prvek->RPtr=NULL;
				(*RootPtr)=prvek; // koren bude nas novy prvek
			}
	}

		  /* V případě řešení smažte tento řádek! */
}

/*                                  PREORDER                                  */

void Leftmost_Preorder (tBTNodePtr ptr, tStackP *Stack)	{
/*   -----------------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Preorder navštívené uzly zpracujeme voláním funkce BTWorkOut()
** a ukazatele na ně is uložíme do zásobníku.
**/

	while(ptr !=NULL)
	{
		SPushP(Stack, ptr); //pushnuti daneho uzlu na zasobnik
		BTWorkOut(ptr); // zpracovani
		ptr=ptr->LPtr; // posunuti doleva
	}
	  /* V případě řešení smažte tento řádek! */
}

void BTPreorder (tBTNodePtr RootPtr)	{
/*   ----------
** Průchod stromem typu preorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Preorder a zásobníku ukazatelů. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut().
**/

	tBTNodePtr prvek=RootPtr; // Init
	tStackP stackP;
	SInitP(&stackP);
	Leftmost_Preorder(prvek, &stackP); // zavolani predchozi funkce, ktera uzly ulozi na stack

	while(!SEmptyP(&stackP)) // dokud zasobnik neni prazdny
	{
		prvek = STopPopP(&stackP);
		//BTWorkOut(prvek);
		Leftmost_Preorder(prvek->RPtr, &stackP);
	}
	  /* V případě řešení smažte tento řádek! */
}


/*                                  INORDER                                   */

void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)		{
/*   ----------------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Inorder ukládáme ukazatele na všechny navštívené uzly do
** zásobníku.
**/

	while(ptr !=NULL) // dokud ptr neni NULL
	{
		SPushP(Stack, ptr); // pushnuti na zasobnik
		ptr=ptr->LPtr; // posunuti do leveho podstromu
	}
		  /* V případě řešení smažte tento řádek! */

}

void BTInorder (tBTNodePtr RootPtr)	{
/*   ---------
** Průchod stromem typu inorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Inorder a zásobníku ukazatelů. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut().
**/

	tBTNodePtr prvek; // Init
	tStackP stackP; // Init
	SInitP(&stackP); // Init
	Leftmost_Inorder(RootPtr, &stackP); // ulpzime uzly na zasobnik

	while( !SEmptyP(&stackP)) // dokud na zasobniku neco je
	{
		prvek = STopPopP(&stackP); // zpracujeme Uzel
		BTWorkOut(prvek);
		Leftmost_Inorder(prvek->RPtr, &stackP);
	}

	  /* V případě řešení smažte tento řádek! */
}

/*                                 POSTORDER                                  */

void Leftmost_Postorder (tBTNodePtr ptr, tStackP *StackP, tStackB *StackB) {
/*           --------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Postorder ukládáme ukazatele na navštívené uzly do zásobníku
** a současně do zásobníku bool hodnot ukládáme informaci, zda byl uzel
** navštíven poprvé a že se tedy ještě nemá zpracovávat.
**/

	while(ptr != NULL) //dokud neni NULL
	{
		SPushP(StackP,ptr); // pushnuti na zasobnik
		SPushB(StackB,true); // pushnuti true hodnoty na zasobnik
		ptr=ptr->LPtr; // posuneme se na levou cast
	}
	  /* V případě řešení smažte tento řádek! */
}

void BTPostorder (tBTNodePtr RootPtr)	{
/*           -----------
** Průchod stromem typu postorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Postorder, zásobníku ukazatelů a zásobníku hotdnot typu bool.
** Zpracování jednoho uzlu stromu realizujte jako volání funkce BTWorkOut().
**/
if(RootPtr==NULL) //pkud je ukazatel RootPtr ukazuje do null, ukonci
{
	return;
}
	tStackB stackB; // init zasobniku
	tStackP stackP;

	SInitB(&stackB);
	SInitP(&stackP);

	bool left;

	Leftmost_Postorder(RootPtr,&stackP, &stackB); //prozeneme funkci Leftmost_Postorder, ulozime na zasobnik
	while(!SEmptyP(&stackP)) // dokud v zasobniku neco je
	{
		tBTNodePtr prvek = STopPopP(&stackP); // do prvek popneme uzel ze zasobniku
		SPushP(&stackP, prvek); // vlozeni uzlu do zasobniku
		left = STopPopB(&stackB);
		if(left) //pujdeme zleva
		{
			SPushB(&stackB, false); // vlozeni false na dalsi pruchod
			Leftmost_Postorder(prvek->RPtr, &stackP, &stackB); // ziskani korene, ktere jsou nyny na zasobniku
		}
			else
			{
				STopPopP(&stackP);//popneme zasobnik
				BTWorkOut(prvek); // zpracujeme/vypiseme
			}
		}

	}

 /* V případě řešení smažte tento řádek! */


void BTDisposeTree (tBTNodePtr *RootPtr)	{
/*   -------------
** Zruší všechny uzly stromu a korektně uvolní jimi zabranou paměť.
**
** Funkci implementujte nerekurzivně s využitím zásobníku ukazatelů.
**/

	//tBTNodePtr prvek;
	tStackP *StackP = (tStackP *) malloc(sizeof(tStackP));
	//tStackB stackP;
	SInitP(StackP);

	do // pracuj dokud plati podminka while
	{
		if((*RootPtr)==NULL)
		{
			if(!SEmptyP(StackP)) // pokud v zasobniku neco je
			{
				(*RootPtr)=STopPopP(StackP); // popneme vrchol zasobniku
			}
		}
		else // pokud RootPtr neni null, cili tam neco je
		{
			if((*RootPtr)->RPtr !=NULL) // pokud ukazatel na pravy uzel je validni
			{
				SPushP(StackP,(*RootPtr)->RPtr);  // tak ho vlozim do zasobniku
			}
			tBTNodePtr aux = (*RootPtr); // do aux ulozim koren
			(*RootPtr) = (*RootPtr)->LPtr; // do korene ulozime levy podstrom
			free(aux); // zrusim auktualni uzel
		}
	}
	while((*RootPtr)!=NULL || !SEmptyP(StackP)); // dokud zasobnik neni prazdny a root neni NULL
	free(StackP); // uvolnim uzel v zasobniku

		  /* V případě řešení smažte tento řádek! */
}

/* konec c402.c */
