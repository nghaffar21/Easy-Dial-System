#include "call_registry.hpp"

/* Construeix un call_registry buit. 
Cost Temporal: O(1) */
call_registry::call_registry() throw(error) {
	_arrel = nullptr;
	_size = 0;
}

/* Constructor per còpia, operador d'assignació i destructor.
Cost Temporal: O(n), on n es el número de nodes de l'arbre */
call_registry::call_registry(const call_registry& R) throw(error){

	_arrel = nullptr;
	_size = 0;
	*this = R;
	
}

//Cost Temporal: O(n), on n es el número de nodes de l'arbre.
call_registry& call_registry::operator=(const call_registry& R) throw(error){
	_size = R._size;
	esborra_nodes(_arrel);
	_arrel = copiar_arbre(R._arrel);
    return *this; // Devuelve una referencia a este objeto	
}

//Cost Temporal: O(n), on n es el número de nodes de l'arbre.
call_registry::node* call_registry::copiar_arbre(node* c){
	node *p = nullptr;
	
	if(c != nullptr) {
		p = new node;
		p->_p = c->_p;
		p->_dret = nullptr;
		p->_esq = nullptr;
		p->_h = c->_h;
		if(c->_esq != nullptr)
			p->_esq = copiar_arbre(c->_esq);
		if(c->_dret != nullptr)
			p->_dret = copiar_arbre(c->_dret);
	}
	return p;
}

//Cost Temporal: O(n), on n es el número de nodes de l'arbre.
call_registry::~call_registry() throw(){
	esborra_nodes(_arrel);
}

//Cost Temporal: O(n), on n es el número de nodes de l'arbre.
void call_registry::esborra_nodes(node* m){
	if (m != nullptr) {
		esborra_nodes(m->_esq);
		esborra_nodes(m->_dret);
		delete m;
	}
}


/* Registra que s'ha realitzat una trucada al número donat, 
incrementant en 1 el comptador de trucades associat. Si el número no 
estava prèviament en el call_registry afegeix una nova entrada amb 
el número de telèfon donat, l'string buit com a nom i el comptador a 1.
Cost Temporal: O(log n), on n es el número de nodes de l'arbre. */
void call_registry::registra_trucada(nat num) throw(error){
	_arrel = registra_trucada(num, _arrel);
}


//Cost Temporal: O(log n), on n es el número de nodes de l'arbre.
call_registry::node* call_registry::registra_trucada(nat num, node* ptr) throw(error){
	
	// Inserció
	if (ptr == nullptr) {
		phone p(num,"",1);
		node* aux = new node;
		aux->_p = p;
		aux->_dret = nullptr;
		aux->_esq = nullptr;
		aux->_h = 1;
		++_size;
		return aux;
	}
	else if (num < ptr->_p.numero())
		ptr->_esq = registra_trucada(num, ptr->_esq);
	else if (num > ptr->_p.numero())
		ptr->_dret = registra_trucada(num, ptr->_dret);
	else {
		++(ptr-> _p);
		return ptr;
	}
	
	ptr->_h = max(altura(ptr -> _esq), altura(ptr -> _dret)) + 1;
	
	int fe = factor_equilibri(ptr);
	
	// Cas EE
	if(fe > 1 && num < ptr->_esq->_p.numero())
		return rotacio_dreta (ptr);
	// Cas DD
	if(fe < -1 && num > ptr->_dret->_p.numero())
		return rotacio_esquerra (ptr);
	// Cas ED
	if(fe > 1 && num > ptr->_esq->_p.numero()) {
		ptr->_esq = rotacio_esquerra(ptr->_esq);
		return rotacio_dreta (ptr);
	}
	// Cas DE
	if (fe < -1 && num < ptr->_dret->_p.numero()) {
		ptr->_dret = rotacio_dreta (ptr->_dret);
		return rotacio_esquerra (ptr);
	}
	return ptr;
}

//Cost Temporal: O(1)
call_registry::node* call_registry::rotacio_dreta(node *y) {
	node *x = y->_esq;
	node *T2 = x->_dret;
	// Realitzem la rotació
	x->_dret = y;
	y->_esq = T2;
	// Actualitzem les altures
	y->_h = max(altura(y->_esq),altura(y->_dret)) + 1;
	x->_h = max(altura(x->_esq),altura(x->_dret)) + 1;
	// Retornem la nova arrel
	return x;
}

//Cost Temporal: O(1)
call_registry::node* call_registry::rotacio_esquerra(node *x) {
	node *y = x->_dret;
	node *T2 = y->_esq;
	// Realitzem la rotació
	y->_esq = x;
	x->_dret = T2;
	// Actualitzem les altures
	x->_h = max(altura(x->_esq),altura(x->_dret)) + 1;
	y->_h = max(altura(y->_esq),altura(y->_dret)) + 1;
	// Retornem la nova arrel
	return y;
}

//Cost Temporal : O(1)
int call_registry::factor_equilibri(node *n) {
	if (n == nullptr) return 0;
	return altura(n-> _esq) - altura(n-> _dret);
}

//Cost Temporal : O(1)
int call_registry::altura(node *n) {
	
	if(n == nullptr)
		return 0;
	return n -> _h;
}

/* Assigna el nom indicat al número donat.
Si el número no estava prèviament en el call_registry, s'afegeix
una nova entrada amb el número i nom donats, i el comptador 
de trucades a 0. 
Si el número existia prèviament, se li assigna el nom donat.
Cost Temporal: O(log n), on n es el número de nodes de l'arbre.*/
void call_registry::assigna_nom(nat num, const string& name) throw(error){
	
	_arrel = assigna_nom(num, name, _arrel);

}

//Cost Temporal: O(log n), on n es el número de nodes de l'arbre.
call_registry::node* call_registry::assigna_nom(nat num, const string& name, node* ptr) throw(error) {

	if (ptr == nullptr) {
		phone p(num,name,0);
		node* aux = new node;
		aux->_p = p;
		aux->_dret = nullptr;
		aux->_esq = nullptr;
		aux->_h = 1;
		++_size;
		return aux;
	}
	else if (num < ptr->_p.numero())
		ptr->_esq = assigna_nom(num, name, ptr->_esq);
	else if (num > ptr->_p.numero())
		ptr->_dret = assigna_nom(num, name, ptr->_dret);
	else {
		nat freq = ptr -> _p.frequencia();
		phone aux(num, name, freq);
		ptr -> _p = aux;
		return ptr;
	}
	
	ptr->_h = max(altura(ptr->_esq), altura(ptr->_dret)) + 1;
	
	int fe = factor_equilibri(ptr);
	
	// Cas EE
	if(fe > 1 && num < ptr->_esq->_p.numero())
		return rotacio_dreta (ptr);
	// Cas DD
	if(fe < -1 && num > ptr->_dret->_p.numero())
		return rotacio_esquerra (ptr);
	// Cas ED
	if(fe > 1 && num > ptr->_esq->_p.numero()) {
		ptr->_esq = rotacio_esquerra(ptr->_esq);
		return rotacio_dreta (ptr);
	}
	// Cas DE
	if (fe < -1 && num < ptr->_dret->_p.numero()) {
		ptr->_dret = rotacio_dreta (ptr->_dret);
		return rotacio_esquerra (ptr);
	}
	return ptr;
	
}

/* Elimina l'entrada corresponent al telèfon el número de la qual es dóna.
Es produeix un error si el número no estava present.
Cost Temporal: O(log n), on n es el número de nodes de l'arbre. */
void call_registry::elimina(nat num) throw(error){
	
	_arrel = elimina(num, _arrel);
	
}

//Cost Temporal: O(log n), on n es el número de nodes de l'arbre.
call_registry::node* call_registry::elimina(nat num, node* ptr) throw(error){

	node *p = ptr;
	
	if (ptr != nullptr) {
		if (num < ptr->_p.numero()) {
			ptr -> _esq = elimina(num, ptr->_esq);
		}
		else if (num > ptr->_p.numero()) {
			ptr -> _dret = elimina(num, ptr->_dret);
		}
		else {
			ptr = ajunta(ptr->_esq , ptr-> _dret );
			delete(p);
			--_size;
		}
	} else {
		throw error(ErrNumeroInexistent);
	}
	
	if(_size == 0) ptr = nullptr;
	
	if(ptr != nullptr) {
	
		ptr->_h = max(altura(ptr -> _esq), altura(ptr -> _dret)) + 1;

		int fe = factor_equilibri(ptr);
				
		// Cas EE
		if(fe > 1 && factor_equilibri(ptr -> _esq) >= 0) {
			return rotacio_dreta (ptr);
		}
			
		// Cas DD
		if(fe < -1 && factor_equilibri(ptr -> _dret) <= 0) {
			return rotacio_esquerra (ptr);
		}
			
		// Cas ED
		if(fe > 1 && factor_equilibri(ptr -> _esq) < 0) {
			ptr->_esq = rotacio_esquerra(ptr->_esq);
			return rotacio_dreta (ptr);
		}
		// Cas DE
		if (fe < -1 && factor_equilibri(ptr -> _dret) > 0) {
			ptr->_dret = rotacio_dreta (ptr->_dret);
			return rotacio_esquerra (ptr);
		}
	}
	
	return ptr;
	
}

/* Cost Temporal: O(h), on h és l'altura de l'arbre t1. En el pitjor cas, això pot ser O(log n) si t1
és un subarbre equilibrat, on n és el nombre de nodes en t1.*/
call_registry::node* call_registry::ajunta( node *t1 , node *t2) throw () {
	if (t1 == nullptr) {
		return t2;
	}
	if (t2 == nullptr) {
		return t1;
	}
	node * p = elimina_maxim(t1);
	p-> _dret = t2;
	return p;
}
/* Cost Temporal: O(h), on h és l'altura de l'arbre representat per p. Això és típicament O(log n) per a un subarbre
equilibrat AVL, on n és el nombre de nodes en el subarbre.*/
call_registry::node* call_registry::elimina_maxim ( node * p) throw () {
	node * p_orig = p, * pare = nullptr ;
	while (p-> _dret != nullptr ) {
		pare = p;
		p = p-> _dret ;
	}
	if ( pare != nullptr ) {
		pare -> _dret = p-> _esq ;
		p-> _esq = p_orig ;
	}
	return p;
}

/* Retorna cert si i només si el call_registry conté un 
telèfon amb el número donat. 
Cost Temporal: O(log n), on n es el número de nodes de l'arbre.*/
bool call_registry::conte(nat num) const throw(){
	
	return conte(num, _arrel);

}

//Cost Temporal: O(log n), on n es el número de nodes de l'arbre. 
bool call_registry::conte(nat num, node *ptr) const throw(){

	if(ptr == nullptr) return false;
	if (ptr -> _p.numero() == num) return true;
	else {
		if (num < ptr->_p.numero()) return conte(num, ptr->_esq);
		else return conte(num, ptr->_dret);
	}
}

/* Retorna el nom associat al número de telèfon que s'indica.
Aquest nom pot ser l'string buit si el número de telèfon no
té un nom associat. Es produeix un error si el número no està en
el call_registry.
Cost Temporal: O(log n), on n es el número de nodes de l'arbre.*/
string call_registry::nom(nat num) const throw(error){
	return nom(num, _arrel);
}


//Cost Temporal: O(log n), on n es el número de nodes de l'arbre.
string call_registry::nom(nat num, node *ptr) const throw(error){
	if(ptr == nullptr) throw error(ErrNumeroInexistent);
	if (ptr -> _p.numero() == num) return ptr->_p.nom();
	else {
		if (num < ptr->_p.numero()) return nom(num, ptr->_esq);
		else return nom(num, ptr->_dret);
	}
}

/* Retorna el comptador de trucades associat al número de telèfon 
indicat. Aquest número pot ser 0 si no s'ha efectuat cap trucada a
aquest número. Es produeix un error si el número no està en el 
call_registry.
Cost Temporal: O(log n), on n es el número de nodes de l'arbre.*/
nat call_registry::num_trucades(nat num) const throw(error){
	return num_trucades(num, _arrel);
}

//Cost Temporal: O(log n), on n es el número de nodes de l'arbre.
nat call_registry::num_trucades(nat num, node *ptr) const throw(error){
	if(ptr == nullptr) throw error(ErrNumeroInexistent); //falta hacer lo del error
	if (ptr -> _p.numero() == num) return ptr->_p.frequencia();
	else {
		if (num < ptr->_p.numero()) return num_trucades(num, ptr->_esq);
		else return num_trucades(num, ptr->_dret);
	}
}

/* Retorna cert si i només si el call_registry està buit. 
Cost Temporal: O(1) */
bool call_registry::es_buit() const throw(){
	
	return (_size == 0);
	
}

/* Retorna quants números de telèfon hi ha en el call_registry. 
Cost Temporal: O(1) */
nat call_registry::num_entrades() const throw(){
	return _size;

}

/* Fa un bolcat de totes les entrades que tenen associat un
nom no nul sobre un vector de phone.
Comprova que tots els noms dels telèfons siguin diferents; 
es produeix un error en cas contrari.
Cost Temporal: O(n), on n es el número de nodes de l'arbre.*/
void call_registry::dump(vector<phone>& V) const throw(error){
	
	if(!es_buit()) {
		
		dump(V, _arrel);
		
		ordena_per_fusio(V);
		if(!V.empty()) {
			for(int i = 0; i<V.size()-1; i++) {
				if(V[i].nom() == V[i+1].nom()) throw error(ErrNomRepetit);
			}
		}
		
	}
}

//Cost Temporal: O(n), on n es el número de nodes de l'arbre.
void call_registry::dump(vector<phone>& V, node *ptr) const throw(error){
	if(ptr != nullptr) {
		if(ptr->_esq != nullptr)
			dump(V, ptr->_esq);
		if(ptr->_p.nom() != "")
			V.push_back(ptr->_p);
		if(ptr->_dret != nullptr)
			dump(V, ptr->_dret);
	}
}

//Funció per fusionar dos subvectors
//Precondició: L[0..n1-1] i R[0..n2-1] són subvectors ordenats de V[esq..dret]
//Postcondició: V[esq..dret] és la fusió ordenada de L i R
//Cost Temporal: O(n), on n és el nombre d'elements en el segment de V[esq..dret]
void call_registry::merge(vector<phone>& V, int esq, int mig, int dret) const {
    int n1 = mig - esq + 1;
    int n2 = dret - mig;

    // Crear vectors temporals
    vector<phone> L(n1), R(n2);

    // Copiar les dades als vectors temporals
    for (int i = 0; i < n1; i++)
        L[i] = V[esq + i];
    for (int j = 0; j < n2; j++)
        R[j] = V[mig + 1 + j];

    // Fusionar els vectors temporals de nou en V
    int i = 0, j = 0, k = esq;
    while (i < n1 && j < n2) {
        if (L[i].nom() <= R[j].nom()) {
            V[k] = L[i];
            i++;
        } else {
            V[k] = R[j];
            j++;
        }
        k++;
    }

    // Copiar els elements restants de L, si n'hi ha
    while (i < n1) {
        V[k] = L[i];
        i++;
        k++;
    }

    // Copiar els elements restants de R, si n'hi ha
    while (j < n2) {
        V[k] = R[j];
        j++;
        k++;
    }
}

//Funció auxiliar per a l'ordenació per fusió
//mergeSort: Ordena un segment d'un vector utilitzant l'ordenació per fusió
//Precondició: esq i dret són els índexs dels extrems del segment de V que es vol ordenar
//Postcondició: El segment V[esq..dret] del vector V està ordenat
//Cost Temporal: O(n log n), on n és el nombre d'elements en el segment de V[esq..dret]
void call_registry::mergeSort(vector<phone>& V, int esq, int dret) const {
    if (esq < dret) {
        // Trobar el punt mitjà
        int mig = esq + (dret - esq) / 2;

        // Ordenar la primera i la segona meitat
        mergeSort(V, esq, mig);
        mergeSort(V, mig + 1, dret);

        // Fusionar les dues meitats ordenades
        merge(V, esq, mig, dret);
    }
}

//ordena_per_fusio: Ordena un vector utilitzant l'ordenació per fusió
//Precondició: V és un vector de doubles
//Postcondició: El vector V està ordenat de petit a gran
//Cost Temporal: O(n log n), on n és el nombre d'elements en V
void call_registry::ordena_per_fusio(vector<phone>& V) const {
    if (!V.empty()) {
        mergeSort(V, 0, V.size() - 1);
    }
}
