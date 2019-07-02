
/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS
**                                         Vytvořil: Petr Přikryl, listopad 1994
**                                         Úpravy: Andrea Němcová, prosinec 1995
**                                                      Petr Přikryl, duben 1996
**                                                   Petr Přikryl, listopad 1997
**                                  Převod do jazyka C: Martin Tuček, říjen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                                 Karel Masařík, říjen 2013
**                                                 Radek Hranický 2014-2018
**
** Implementujte rekurzivním způsobem operace nad binárním vyhledávacím
** stromem (BVS; v angličtině BST - Binary Search Tree).
**
** Klíčem uzlu stromu je jeden znak (obecně jím může být cokoliv, podle
** čeho se vyhledává). Užitečným (vyhledávaným) obsahem je zde integer.
** Uzly s menším klíčem leží vlevo, uzly s větším klíčem leží ve stromu
** vpravo. Využijte dynamického přidělování paměti.
** Rekurzivním způsobem implementujte následující funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíčem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zrušení uzlu se zadaným klíčem
**   BSTDispose ... zrušení celého stromu
**
** ADT BVS je reprezentován kořenovým ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klíč (typu char), podle
** kterého se ve stromu vyhledává, vlastní obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levý a pravý podstrom (LPtr a RPtr). Přesnou definici typů
** naleznete v souboru c401.h.
**
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c401.h"
int solved;

void BSTInit (tBSTNodePtr *RootPtr) {
/*   -------
** Funkce provede počáteční inicializaci stromu před jeho prvním použitím.
**
** Ověřit, zda byl již strom předaný přes RootPtr inicializován, nelze,
** protože před první inicializací má ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Programátor využívající ADT BVS tedy musí zajistit, aby inicializace
** byla volána pouze jednou, a to před vlastní prací s BVS. Provedení
** inicializace nad neprázdným stromem by totiž mohlo vést ke ztrátě přístupu
** k dynamicky alokované paměti (tzv. "memory leak").
**
** Všimněte si, že se v hlavičce objevuje typ ukazatel na ukazatel.
** Proto je třeba při přiřazení přes RootPtr použít dereferenční operátor *.
** Ten bude použit i ve funkcích BSTDelete, BSTInsert a BSTDispose.
**/


	 (*RootPtr)=NULL;	  /* V případě řešení smažte tento řádek! */

}

int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{
/*  ---------
** Funkce vyhledá uzel v BVS s klíčem K.
**
** Pokud je takový nalezen, vrací funkce hodnotu TRUE a v proměnné Content se
** vrací obsah příslušného uzlu.´Pokud příslušný uzel není nalezen, vrací funkce
** hodnotu FALSE a obsah proměnné Content není definován (nic do ní proto
** nepřiřazujte).
**
** Při vyhledávání v binárním stromu bychom typicky použili cyklus ukončený
** testem dosažení listu nebo nalezení uzlu s klíčem K. V tomto případě ale
** problém řešte rekurzivním volání této funkce, přičemž nedeklarujte žádnou
** pomocnou funkci.
**/

	if(RootPtr==NULL) // nenalezen
	{
		return FALSE;
	}
	else
	{
		if(RootPtr->Key==K) // nasli jsme Uzel
		{
			*Content=RootPtr->BSTNodeCont;
			return TRUE;
		}
		else if(RootPtr->Key > K) //hledame v pravem podstromu
		{
			return (BSTSearch(RootPtr->LPtr,K, Content));
		}
		else if(RootPtr->Key < K) // hledame v levem podstromu
		{
			return (BSTSearch(RootPtr->RPtr, K, Content));
		}
		else
		return FALSE;
	}
		  /* V případě řešení smažte tento řádek! */
}


void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{
/*   ---------
** Vloží do stromu RootPtr hodnotu Content s klíčem K.
**
** Pokud již uzel se zadaným klíčem ve stromu existuje, bude obsah uzlu
** s klíčem K nahrazen novou hodnotou. Pokud bude do stromu vložen nový
** uzel, bude vložen vždy jako list stromu.
**
** Funkci implementujte rekurzivně. Nedeklarujte žádnou pomocnou funkci.
**
** Rekurzivní implementace je méně efektivní, protože se při každém
** rekurzivním zanoření ukládá na zásobník obsah uzlu (zde integer).
** Nerekurzivní varianta by v tomto případě byla efektivnější jak z hlediska
** rychlosti, tak z hlediska paměťových nároků. Zde jde ale o školní
** příklad, na kterém si chceme ukázat eleganci rekurzivního zápisu.
**/

	if(*RootPtr==NULL)
	{
		tBSTNodePtr prvek = malloc(sizeof(struct tBSTNode));
		if(prvek==NULL) // pokud malloc nevysel
		{
			return;
		}
		else
		{
			(prvek)->BSTNodeCont=Content;
			(prvek)->Key=K;
			(prvek)->LPtr=NULL;
			(prvek)->RPtr=NULL;
			(*RootPtr)=prvek;
		}
	}
	else
	{
		if((*RootPtr)->Key == K) // pokud je tam stejny Uzel
		{
			(*RootPtr)->BSTNodeCont=Content;
		}
		else if((*RootPtr)->Key > K)
		{
			BSTInsert(&(*RootPtr)->LPtr,K, Content); // pokud je vkladany uzel mensi nez porovnavany, budeme hledat v leve vetvy
		}
		else if((*RootPtr)->Key < K)
		{
			BSTInsert(&(*RootPtr)->RPtr, K, Content); // pokud je vkladany uzel mensi nez porovnavany, budeme hledat v prave vetvy
		}
	}
	  /* V případě řešení smažte tento řádek! */

}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
/*   ------------------
** Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterého bude přesunuta hodnota
** nejpravějšího uzlu v podstromu, který je určen ukazatelem RootPtr.
** Předpokládá se, že hodnota ukazatele RootPtr nebude NULL (zajistěte to
** testováním před volání této funkce). Tuto funkci implementujte rekurzivně.
**
** Tato pomocná funkce bude použita dále. Než ji začnete implementovat,
** přečtěte si komentář k funkci BSTDelete().
**/

	tBSTNodePtr prvek;
		if((*RootPtr)!=NULL)
		{
			if((*RootPtr)->RPtr==NULL) // nyni mame nejpravejsi uzel, tzn. nema zadne praveho potmka
			{
				PtrReplaced->BSTNodeCont=(*RootPtr)->BSTNodeCont; // nahrajeme data a key
				PtrReplaced->Key=(*RootPtr)->Key;
				prvek=(*RootPtr); // ulozime si hodnotu rootu
				(*RootPtr)=(*RootPtr)->LPtr; //prepojime
				free(prvek); // uvolnime nejpravejsi Uzel
			}
			else // pokud nemame nejpravejsi uzel, funkci zavolame znovu
			{
				ReplaceByRightmost(PtrReplaced, &(*RootPtr)->RPtr);
			}
		}
		  /* V případě řešení smažte tento řádek! */

}

void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
/*   ---------
** Zruší uzel stromu, který obsahuje klíč K.
**
** Pokud uzel se zadaným klíčem neexistuje, nedělá funkce nic.
** Pokud má rušený uzel jen jeden podstrom, pak jej zdědí otec rušeného uzlu.
** Pokud má rušený uzel oba podstromy, pak je rušený uzel nahrazen nejpravějším
** uzlem levého podstromu. Pozor! Nejpravější uzel nemusí být listem.
**
** Tuto funkci implementujte rekurzivně s využitím dříve deklarované
** pomocné funkce ReplaceByRightmost.
**/


	if((*RootPtr)!=NULL)
	{

		if((*RootPtr)->Key==K) // klic jsme nasli
		{
			tBSTNodePtr prvek;
			if(((*RootPtr)->LPtr == NULL) && ((*RootPtr)->RPtr == NULL)) // pokud nejsou zadne listy
			{
				free(*RootPtr);
				(*RootPtr)=NULL;
			}
			else if((*RootPtr)->LPtr==NULL && ((*RootPtr)->RPtr!=NULL)) // pokud ma pouze pravy list
			{
				prvek=(*RootPtr);
				(*RootPtr)=(*RootPtr)->RPtr; // do uzlu, ktery se maze, vlozime jeho pravy uzel
				free(prvek); // uzel smazeme
			}
			else if((*RootPtr)->LPtr!=NULL && ((*RootPtr)->RPtr==NULL)) // pokud ma pouze levy list
			{
				prvek=(*RootPtr);
				(*RootPtr)=(*RootPtr)->LPtr; //obdoba predchoziho ifu
				free(prvek);
			}
			else if((*RootPtr)->LPtr!=NULL && ((*RootPtr)->RPtr!=NULL)) // pokud ma dva potomky, v prdeli..
			{
				ReplaceByRightmost(*RootPtr, (&(*RootPtr)->LPtr));
			}
		}
		else if((*RootPtr)->Key > K) // projedeme levy podstrom
		{
			BSTDelete(&(*RootPtr)->LPtr, K);
		}
		else if ((*RootPtr)->Key < K) // projedeme pravy podstromu
		{
			BSTDelete(&(*RootPtr)->RPtr, K);
		}
	}
 // mozna jeste neco !!!!

	  /* V případě řešení smažte tento řádek! */

}

void BSTDispose (tBSTNodePtr *RootPtr) {
/*   ----------
** Zruší celý binární vyhledávací strom a korektně uvolní paměť.
**
** Po zrušení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivně bez deklarování pomocné
** funkce.
**/
	//tBSTNodePtr prvek;
	if((*RootPtr)!=NULL)
	{
		if((*RootPtr)->LPtr!=NULL) // pokud ma leveho
		{
			BSTDispose(&(*RootPtr)->LPtr);
		}
		else if((*RootPtr)->RPtr!=NULL) // pokud ma praveho potomka
		{
			BSTDispose(&(*RootPtr)->RPtr);
		}
	}
	//BSTDispose(RootPtr);
	free(*RootPtr);
	*RootPtr=NULL;
		  /* V případě řešení smažte tento řádek! */

}

/* konec c401.c */
