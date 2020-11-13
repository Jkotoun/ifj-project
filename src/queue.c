#include "headers/queue.h"

void queueError(int error_code)
{
    /*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**
** TUTO FUNKCI, PROSÍME, NEUPRAVUJTE!
*/
    static const char* QERR_STRINGS[MAX_QERR + 1] = { "Unknown error", "Queue error: UP", "Queue error: FRONT", "Queue error: REMOVE", "Queue error: GET", "Queue error: INIT" };
    if (error_code <= 0 || error_code > MAX_QERR)
        error_code = 0;
    printf("%s\n", QERR_STRINGS[error_code]);
    err_flag = 1;
}

void typeQueueInit(typeQueue* q)
{
    /*
** Inicializujte frontu následujícím způsobem:
** - všechny hodnoty v poli q->arr nastavte na '*',
** - index na začátek fronty nastavte na 0,
** - index prvního volného místa nastavte také na 0.
**
** V případě, že funkce dostane jako parametr q == NULL, volejte funkci
** queueError(QERR_INIT).
*/
    if (!q)
    {
        queueError(QERR_INIT);
        return;
    }
    for (size_t i = 0; i < MAX_QUEUE; i++)
    {
        q->arr[i] = 0;
    }
    q->f_index = 0;
    q->b_index = 0;
}

int nextIndex(int index)
{
    /*
** Pomocná funkce, která vrací index následujícího prvku v poli.
** Funkci implementujte jako jediný prikaz využívající operace '%'.
** Funkci nextIndex budete využívat v dalších implementovaných funkcích.
*/

    return (index + 1) % MAX_QUEUE;
}

int typeQueueEmpty(const typeQueue* q)
{
    /*
** Vrací nenulovou hodnotu, pokud je frona prázdná, jinak vrací hodnotu 0.
** Funkci je vhodné implementovat jedním příkazem return.
*/

    return q->f_index == q->b_index;
}

int typeQueueFull(const typeQueue* q)
{
    /*
** Vrací nenulovou hodnotu, je-li fronta plná, jinak vrací hodnotu 0.
** Funkci je vhodné implementovat jedním příkazem return
** s využitím pomocné funkce nextIndex.
*/

    return nextIndex(q->b_index) == q->f_index;
}

void typeQueueFront(const typeQueue* q, varType* c)
{
    /*
** Prostřednictvím parametru c vrátí znak ze začátku fronty q.
** Pokud je fronta prázdná, ošetřete to voláním funkce queueError(QERR_FRONT).
** Volání této funkce při prázdné frontě je vždy nutné považovat za nekorektní.
** Bývá to totiž důsledek špatného návrhu algoritmu, ve kterém je fronta
** použita. O takové situaci se proto musí programátor-vývojář dozvědět.
** V opačném případě je ladění programů obtížnější!
**
** Při implementaci využijte dříve definované funkce typeQueueEmpty.
*/
    if (typeQueueEmpty(q))
    {
        queueError(QERR_FRONT);
        return;
    }
    *c = q->arr[q->f_index];
}

void typeQueueRemove(typeQueue* q)
{
    /*
** Odstraní znak ze začátku fronty q. Pokud je fronta prázdná, ošetřete
** vzniklou chybu voláním funkce queueError(QERR_REMOVE).
** Hodnotu na uvolněné pozici ve frontě nijak neošetřujte (nepřepisujte).
** Při implementaci využijte dříve definované funkce typeQueueEmpty a nextIndex.
*/
    if (typeQueueEmpty(q))
    {
        queueError(QERR_REMOVE);
        return;
    }
    q->f_index = nextIndex(q->f_index);
}

void typeQueueGet(typeQueue* q, varType* c)
{
    /*
** Odstraní znak ze začátku fronty a vrátí ho prostřednictvím parametru c.
** Pokud je fronta prázdná, ošetřete to voláním funkce queueError(QERR_GET).
**
** Při implementaci využijte dříve definovaných funkcí typeQueueEmpty,
** typeQueueFront a typeQueueRemove.
*/
    if (typeQueueEmpty(q))
    {
        queueError(QERR_GET);
        return;
    }
    typeQueueFront(q, c);
    typeQueueRemove(q);
}

void typeQueueUp(typeQueue* q, varType c)
{
    /*
** Vloží znak c do fronty. Pokud je fronta plná, ošetřete chybu voláním
** funkce queueError(QERR_UP). Vkládání do plné fronty se považuje za
** nekorektní operaci. Situace by mohla být řešena i tak, že by operace
** neprováděla nic, ale v případě použití takto definované abstrakce by se
** obtížně odhalovaly chyby v algoritmech, které by abstrakci využívaly.
**
** Při implementaci využijte dříve definovaných funkcí typeQueueFull a nextIndex.
*/
    if (typeQueueFull(q))
    {
        queueError(QERR_UP);
        return;
    }
    q->arr[q->b_index] = c;
    q->b_index = nextIndex(q->b_index);

}

int typeQueueLength(typeQueue* q)
{
    return q->b_index - q->f_index;
}

varType* typeQueueToArray(typeQueue* q)
{
    int size = typeQueueLength(q);
    if (size == 0)
    {
        return NULL;
    }
    varType* arr = malloc(sizeof(varType) * size);
    for (size_t i = 0; i < size; i++)
    {
        arr[i] = q->arr[q->f_index + i];
    }
    return arr;
}

// ---------------------------- TOKEN QUEUE ---------------------------------------------

void tokenQueueInit(tokenQueue* q)
{
    /*
** Inicializujte frontu následujícím způsobem:
** - všechny hodnoty v poli q->arr nastavte na '*',
** - index na začátek fronty nastavte na 0,
** - index prvního volného místa nastavte také na 0.
**
** V případě, že funkce dostane jako parametr q == NULL, volejte funkci
** queueError(QERR_INIT).
*/
    if (!q)
    {
        queueError(QERR_INIT);
        return;
    }
    q->f_index = 0;
    q->b_index = 0;
}


int tokenQueueEmpty(const tokenQueue* q)
{
    /*
** Vrací nenulovou hodnotu, pokud je frona prázdná, jinak vrací hodnotu 0.
** Funkci je vhodné implementovat jedním příkazem return.
*/

    return q->f_index == q->b_index;
}

int tokenQueueFull(const tokenQueue* q)
{
    /*
** Vrací nenulovou hodnotu, je-li fronta plná, jinak vrací hodnotu 0.
** Funkci je vhodné implementovat jedním příkazem return
** s využitím pomocné funkce nextIndex.
*/

    return nextIndex(q->b_index) == q->f_index;
}

void tokenQueueFront(const tokenQueue* q, token* token)
{
    /*
** Prostřednictvím parametru c vrátí znak ze začátku fronty q.
** Pokud je fronta prázdná, ošetřete to voláním funkce queueError(QERR_FRONT).
** Volání této funkce při prázdné frontě je vždy nutné považovat za nekorektní.
** Bývá to totiž důsledek špatného návrhu algoritmu, ve kterém je fronta
** použita. O takové situaci se proto musí programátor-vývojář dozvědět.
** V opačném případě je ladění programů obtížnější!
**
** Při implementaci využijte dříve definované funkce typeQueueEmpty.
*/
    if (tokenQueueEmpty(q))
    {
        queueError(QERR_FRONT);
        return;
    }
    *token = q->arr[q->f_index];
}

void tokenQueueRemove(tokenQueue* q)
{
    /*
** Odstraní znak ze začátku fronty q. Pokud je fronta prázdná, ošetřete
** vzniklou chybu voláním funkce queueError(QERR_REMOVE).
** Hodnotu na uvolněné pozici ve frontě nijak neošetřujte (nepřepisujte).
** Při implementaci využijte dříve definované funkce tokenQueueEmpty a nextIndex.
*/
    if (tokenQueueEmpty(q))
    {
        queueError(QERR_REMOVE);
        return;
    }
    q->f_index = nextIndex(q->f_index);
}

void tokenQueueGet(tokenQueue* q, token* token)
{
    /*
** Odstraní znak ze začátku fronty a vrátí ho prostřednictvím parametru c.
** Pokud je fronta prázdná, ošetřete to voláním funkce queueError(QERR_GET).
**
** Při implementaci využijte dříve definovaných funkcí typeQueueEmpty,
** typeQueueFront a typeQueueRemove.
*/
    if (tokenQueueEmpty(q))
    {
        queueError(QERR_GET);
        return;
    }
    tokenQueueFront(q, token);
    tokenQueueRemove(q);
}

void tokenQueueUp(tokenQueue* q, token token)
{
    /*
** Vloží znak c do fronty. Pokud je fronta plná, ošetřete chybu voláním
** funkce queueError(QERR_UP). Vkládání do plné fronty se považuje za
** nekorektní operaci. Situace by mohla být řešena i tak, že by operace
** neprováděla nic, ale v případě použití takto definované abstrakce by se
** obtížně odhalovaly chyby v algoritmech, které by abstrakci využívaly.
**
** Při implementaci využijte dříve definovaných funkcí typeQueueFull a nextIndex.
*/
    if (tokenQueueFull(q))
    {
        queueError(QERR_UP);
        return;
    }
    q->arr[q->b_index] = token;
    q->arr[q->b_index].str = malloc(sizeof(string));
    strInit(q->arr[q->b_index].str);
    strCopyString(q->arr[q->b_index].str, token.str);

    q->b_index = nextIndex(q->b_index);

}

int tokenQueueLength(tokenQueue* q)
{
    return q->b_index - q->f_index;
}

token* tokenQueueToArray(tokenQueue* q)
{
    int size = tokenQueueLength(q);
    if (size == 0)
    {
        return NULL;
    }
    token* arr = malloc(sizeof(token) * size);
    for (size_t i = 0; i < size; i++)
    {
        arr[i] = q->arr[q->f_index + i];
    }
    return arr;
}