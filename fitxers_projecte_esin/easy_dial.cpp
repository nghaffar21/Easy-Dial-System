#include "easy_dial.hpp"

/* Construeix un easy_dial a partir de la
informació continguda en el call_registry donat. El
prefix en curs queda indefinit. 
Cost Temporal: O(n), on n es el número de caràcters de tots els phones */
easy_dial::easy_dial(const call_registry& R) throw(error) : _pvalid(false), _arrel(nullptr), _prefix(""), _suma_freqs(0) {

	vector<phone> v;
	R.dump(v);

	for (int i = 0; i < v.size(); i++) { //guarda els noms dels phones en el trie

		insereix_nom(v[i]);
		_suma_freqs += v[i].frequencia();	// guardar la suma de frequencia de tots els phones(utilitzat en el métode longitud_mitjana)
	}

	ordena_per_fusio(v); //ordenem vector de phones per frequencies

	if (!v.empty()) {
		_max_phone = v[v.size() - 1];

		//guarda els objectes phone dins del trie
		for (int i = v.size() - 2; i >= 0; i--) {	// i < v.size() - 1 porque el telefono que tiene mas frequencia de todo esta en variable _max_phone

			insereix_phone(v[i]);
		}
	}

	_actual = _arrel;
	_actual_valid = true;

}

//Cost Temporal : O(p.nom.length()).
void easy_dial::insereix_phone(const phone& p) {
	insereix_phone(_arrel, p.nom(), 0, p);
}

//Cost Temporal : O(p.nom.length()).
void easy_dial::insereix_phone(node* n, string k, nat i, const phone& p) {
	if (n != nullptr) {

		if ((n->_c) < k[i]) {
			insereix_phone(n->_sg, k, i, p);
		}
		else if ((n->_c) == k[i]) {
			if (n->_p.nom() == "") n->_p = p;
			else insereix_phone(n->_pf, k, i + 1, p);
		}
	}
}


/*constructor por nodo
Cost Temporal : O(1)*/
easy_dial::node::node(const char& c, node* pf, node* sg) : _c(c), _p(), _pf(pf), _sg(sg) {

}

//Cost Temporal: O(log n) i en el pitjor cas O(n), on n és el número de nodes del trie.
void easy_dial::insereix_nom(const phone& p) {
	_arrel = insereix_nom(_arrel, 0, p.nom() + phone::ENDPREF);
}


//Cost Temporal: O(log n) i en el pitjor cas O(n), on n és el número de nodes del trie.
easy_dial::node* easy_dial::insereix_nom(node* t, nat i, const string& k) {
	if (t == nullptr) {
		t = new node(k[i]);

		if (i < k.length() - 1) t->_pf = insereix_nom(t->_pf, i + 1, k);

	}
	else {
		if (t->_c > k[i]) {
			t = new node(k[i], nullptr, t);

			if (i < k.length() - 1) {
				t->_pf = insereix_nom(t->_pf, i + 1, k);
			}

		}
		else if (t->_c < k[i]) {
			t->_sg = insereix_nom(t->_sg, i, k);
		}
		else { 
			t->_pf = insereix_nom(t->_pf, i + 1, k);
		}
	}

	return t;
}


//ordena_per_fusio: Ordena un vector utilitzant l'ordenació per fusió
//Precondició: V és un vector de doubles
//Postcondició: El vector V està ordenat de petit a gran
//Cost Temporal: O(n log n), on n és el nombre d'elements en V
void easy_dial::ordena_per_fusio(vector<phone>& V) const {
	if (!V.empty()) {
		mergeSort(V, 0, V.size() - 1);
	}
}


//Funció auxiliar per a l'ordenació per fusió
//mergeSort: Ordena un segment d'un vector utilitzant l'ordenació per fusió
//Precondició: esq i dret són els índexs dels extrems del segment de V que es vol ordenar
//Postcondició: El segment V[esq..dret] del vector V està ordenat
//Cost Temporal: O(n log n), on n és el nombre d'elements en el segment de V[esq..dret]
void easy_dial::mergeSort(vector<phone>& V, int esq, int dret) const {
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

//Funció per fusionar dos subvectors
//Precondició: L[0..n1-1] i R[0..n2-1] són subvectors ordenats de V[esq..dret]
//Postcondició: V[esq..dret] és la fusió ordenada de L i R
//Cost Temporal: O(n), on n és el nombre d'elements en el segment de V[esq..dret]
void easy_dial::merge(vector<phone>& V, int esq, int mig, int dret) const {
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
		if (L[i].frequencia() <= R[j].frequencia()) {
			V[k] = L[i];
			i++;
		}
		else {
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

/* Tres grans. Constructor per còpia, operador d'assignació i destructor. 
Cost Temporal: O(n), on n és el número de nodes del trie.*/
easy_dial::easy_dial(const easy_dial& D) throw(error) : _pvalid(false), _arrel(nullptr), _prefix(""), _suma_freqs(0) {

	_actual_valid = true;
	*this = D;

}

//Cost Temporal: O(n), on n és el número de nodes del trie.
easy_dial& easy_dial::operator=(const easy_dial& D) throw(error) {

	_pvalid = D._pvalid;

	_actual_valid = D._actual_valid;

	_actual = D._actual;

	_prefix = D._prefix;

	_suma_freqs = D._suma_freqs;

	_max_phone = D._max_phone;
	
	esborra_nodes(_arrel);
	
	_arrel = copiar_arbre(D._arrel);

	return *this; // retorna una referencia a aquest objecte

}

//Métode auxiliar d'operador d'assignació
//Cost Temporal: O(n), on n és el número de nodes del trie.
easy_dial::node* easy_dial::copiar_arbre(node* d) {
	node* p = nullptr;

	if (d != nullptr) {

		p = new node(d->_c);

		p->_p = d->_p;

		p->_pf = p->_sg = nullptr;
		p->_pf = copiar_arbre(d->_pf);
		p->_sg = copiar_arbre(d->_sg);

	}
	return p;
}


//Cost Temporal: O(n), on n és el número de nodes del trie.
easy_dial::~easy_dial() throw() {

	esborra_nodes(_arrel);
	_arrel = nullptr;
}

//Cost Temporal: O(n), on n és el número de nodes del trie.
void easy_dial::esborra_nodes(node* t) {
	if (t != nullptr) {

		esborra_nodes(t->_pf);
		esborra_nodes(t->_sg);
		delete t;
	}
}

/* Inicialitza el prefix en curs a buit. Retorna el nom de F(S, '');
si F (S, '') no existeix llavors retorna l'string buit. 
Cost Temporal: O(1) */
string easy_dial::inici() throw() {
	_actual = _arrel;
	_actual_valid = true;
	_prefix = "";
	_pvalid = true;
	if (_arrel == nullptr) { return ""; }

	return _max_phone.nom();

}

/* Retorna el nom de F(S, p') on p' és el prefix resultant d'afegir
el caràcter c al final del prefix en curs p i
fa que el nou prefix en curs sigui p'.
Si F(S, p) existeix però F(S, p') no existeix llavors retorna
l'string buit.
Si no existeix F(S, p) (i per tant tampoc pot existir F(S, p'))
llavors es produeix un error i el prefix en curs queda indefinit.
Naturalment, es produeix un error si el prefix en curs inicial p
fos indefinit.
Cost Temporal: O(l) on l és el número de lletres de l'abecedari*/
string easy_dial::seguent(char c) throw(error) {

	if (_actual != nullptr) {

		// prefix indefinit

		if (!_pvalid) {

			_prefix = _prefix + c;
			throw error(ErrPrefixIndef);

		}

		// Si no existeix F(S, p) (i per tant tampoc pot existir F(S, p'))
		// llavors es produeix un error i el prefix en curs queda indefinit.

		if (!_actual_valid) {
			_pvalid = false;
			_prefix = _prefix + c;
			throw error(ErrPrefixIndef);
		}

		// Si F(S, p) existeix

		node* aux = nullptr;
		if (_prefix == "") {

			aux = seguent(_actual, c); // busca el node de caracter c

		}
		else if (_actual->_pf != nullptr) aux = seguent(_actual->_pf, c);

		_prefix = _prefix + c;

		if (aux == nullptr or aux->_p.nom() == "") { _actual_valid = false;  return ""; } // Si F(S, p') no existeix, return ""
		else _actual = aux;	

		return _actual->_p.nom(); // el caso que F(S, p') existeix

	}
	else {

		_pvalid = false;
		_prefix = _prefix + c;
		throw error(ErrPrefixIndef);

	}

}

//Cost Temporal: O(l) on l és el número de lletres de l'abecedari
easy_dial::node* easy_dial::seguent(node* ptr, char c) {

	node* aux = nullptr;
	if (ptr != nullptr) {

		if (ptr->_c == c) return ptr;
		aux = seguent(ptr->_sg, c);

	}
	return aux;

}

/* Elimina l'últim caràcter del prefix en curs p = p' · a
(a és el caràcter eliminat). Retorna el nom F(S, p')
i fa que el nou prefix en curs sigui p'.
Es produeix un error si p fos buida i si es fa que el prefix en curs
quedi indefinit. Òbviament, també es produeix un error
si p fos indefinit.
Cost Temporal: O(prefix.length()). */
string easy_dial::anterior() throw(error) {

	if (!_pvalid) throw error(ErrPrefixIndef);

	if (_prefix == "") {

		_pvalid = false;
		throw error(ErrNoHiHaAnterior);

	}

	_prefix.pop_back();

	if (_prefix == "") return inici();
	else {
		string ret_value = anterior(_arrel, 0);
		return ret_value;
	}

}

//Cost Temporal : O(prefix.length()).
string easy_dial::anterior(node* n, nat i) {

	if ((n->_c) < _prefix[i]) {
		return anterior(n->_sg, i);
	}
	else if ((n->_c) == _prefix[i]) {
		if (i == _prefix.length() - 1) {

			_actual = n;
			_actual_valid = true;
			return ((n->_p).nom());

		}
		else return anterior(n->_pf, i + 1);
	}

	return "";

}

/* Retorna el número de telèfon de F(S, p), sent p
el prefix en curs. Es produeix un error si p és indefinit o si
no existeix F(S, p). 
Cost Temporal: O(1) */
nat easy_dial::num_telf() const throw(error) {


	if (!_pvalid) throw error(ErrPrefixIndef);

	// Si no existeix F(S, p)

	if (_prefix == "" and _arrel == nullptr) {		//trie buit

		throw error(ErrNoExisteixTelefon);
	}

	if (!_actual_valid) throw error(ErrNoExisteixTelefon);

	if (_prefix == "") {	//El cas de que prefix es buit y trie no es buit


		return _max_phone.numero();

	}

	return _actual->_p.numero();

}

/* Retorna en el vector result tots els noms dels contactes de
telèfon que comencen amb el prefix pref, en ordre lexicogràfic creixent.
Cost Temporal: Si pref és buit, recorre tot el trie, el que té un cost de O(S) on S és la suma de la longitud de
totes les paraules emmagatzemades. Si pref no és buit, primer busca el node que correspon al final del prefix
(això té un cost de O(P), on P és la longitud del prefix i després realitza un recorregut per trobar totes les coincidències,
el cost del qual depèn de la quantitat i longitud de les coincidències.*/
void easy_dial::comencen(const string& pref, vector<string>& result) const throw(error) {

	// En el cas de que call_registry no està buit
	if (_arrel != nullptr) {

		if (pref == "") {	// si pref es "", afegeix tots els phones del trie

			string nom_tel = "";
			comencen(_arrel, result, pref, nom_tel);

		}
		else { 	// afegeix tots els phones que tinguin prefix

			node* aux = _arrel;
			if (comencen(aux, pref, 0)) {

				if (aux != nullptr) aux = aux->_pf;

				string nom_tel = "";	// guarda el nom que es construeix recorrent el trie
				comencen(aux, result, pref, nom_tel);	// afegeix els phones en el trie

			}

		}
	}
}

/* aux1 - recorregut del trie fins trobar el prefix
Cost temporal: O(prefix.length())*/
bool easy_dial::comencen(node*& n, const string& pref, int i) const {

	if (n != nullptr) {

		if (n->_c > pref[i]) {
			return false;
		}
		else if ((n->_c) < pref[i]) {

			n = n->_sg;
			return comencen(n, pref, i);

		}
		else if ((n->_c) == pref[i]) {
			if (i == pref.length() - 1)
				return true;
			else {

				n = n->_pf;
				return comencen(n, pref, i + 1);
			}
		}
	}

	return false;

}

/* aux2 - recorregut del trie per afegir tots els phones de prefix
Cost temporal: O(M), on M és el nombre total de caràcters en totes les paraules que comencen amb el prefix. */
void easy_dial::comencen(node* ptr, vector<string>& result, const string pref, string nom_tel) const {

	if (ptr != nullptr) {

		nom_tel = nom_tel + (ptr->_c); //nom_tel és el nom de phone que afegim a result

		comencen(ptr->_pf, result, pref, nom_tel);

		if ((ptr->_c) == phone::ENDPREF) result.push_back(pref + nom_tel.substr(0, nom_tel.length() - 1));

		comencen(ptr->_sg, result, pref, nom_tel.substr(0, nom_tel.length() - 1));	//elimina l'últim caràcter del pref

	}
}

/* Retorna el número mitjà de pulsacions necessàries para obtenir un
telèfon. Formalment, si X és el conjunt de noms emmagatzemats en
el easy_dial i t(s) és el número de pulsacions mínimes
necessàries (= número de crides a l'operació seguent) per
obtenir el telèfon el nom del qual és s. La funció retorna la suma
  Pr(s) · t(s)
per tots els telèfons s del conjunt X, sent Pr(s) la probabilitat de
telefonar a s. La probabilitat s'obté dividint la freqüència de s per
la suma de totes les freqüències. 
Cost Temporal: O(n), on n és el número de nodes del trie. */
double easy_dial::longitud_mitjana() const throw() {


	double long_mitj = 0;
	nat num_puls = 1;	// conta el número de pulsacions per arribar a un phone

	return longitud_mitjana(_arrel, long_mitj, num_puls);

}

//Cost Temporal: O(n), on n és el número de nodes del trie.
double easy_dial::longitud_mitjana(node* n, double& long_mitj, nat num_puls) const {

	if (n != nullptr) {

		if ((n->_p).nom() != "") {

			double pr = static_cast<double>(n->_p.frequencia()) / static_cast<double>(_suma_freqs); // Probabilitat Pr(s)

			long_mitj += pr * static_cast<double>(num_puls);

		}

		longitud_mitjana(n->_pf, long_mitj, num_puls + 1);
		longitud_mitjana(n->_sg, long_mitj, num_puls);

	}
	return long_mitj;

}
