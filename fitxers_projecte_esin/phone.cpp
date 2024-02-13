#include "phone.hpp"

  /* Construeix un telèfon a partir del seu número (num), el seu nom
  (name) i el seu comptador de trucades (compt). 
  Es produeix un error si name no és un identificador legal. 
  Cost Temporal : O(1) */
  phone::phone(nat num, const string& name, nat compt) throw(error) {
  	if ( (name.find(DELETECHAR) == std::string::npos) and (name.find(ENDCHAR) == std::string::npos) and (name.find(ENDPREF) == std::string::npos)) {
  		_num = num;
  		_name = name;
  		_compt = compt;
  	}
  	else throw error(ErrNomIncorrecte); 
  }

  /* Tres grans. Constructor per còpia, operador d'assignació i destructor. 
  Cost Temporal : O(1) */
  phone::phone(const phone& T) throw(error) {
	  
	 //if ( (T._name.find(DELETECHAR) == std::string::npos) and (T._name.find(ENDCHAR) == std::string::npos) and (T._name.find(ENDPREF) == std::string::npos)) {
		 
		 *this = T;
		 
	 //}
	  
 	 //else throw error(ErrNomIncorrecte); 
  }
  
  //Cost Temporal : O(1)
  phone& phone::operator=(const phone& T) throw(error) {
  	
  	//if ( (T._name.find(DELETECHAR) == std::string::npos) and (T._name.find(ENDCHAR) == std::string::npos) and (T._name.find(ENDPREF) == std::string::npos)) {
		_num = T._num;  	
		_name = T._name;
		_compt = T._compt;
	//}
  	//else throw error(ErrNomIncorrecte);

  	return *this;
  }
  
  //Cost Temporal : O(1)
  phone::~phone() throw() {
  
  }

  /* Retorna el número de telèfon. 
  Cost Temporal : O(1) */
  nat phone::numero() const throw() {
  	return _num;
  }

  /* Retorna el nom associat al telèfon, eventualment l'string buit.
  Cost Temporal : O(1) */
  string phone::nom() const throw() {
  	return _name;
  }

  /* Retorna el número de vegades que s'ha trucat al telèfon.
  Cost Temporal : O(1) */
  nat phone::frequencia() const throw(){
  	return _compt;
  }

  /* Operador de preincrement. 
  Incrementa en 1 el número de vegades que s'ha trucat al telèfon i
  retorna una referència a aquest telèfon.
  Cost Temporal : O(1) */
  phone& phone::operator++() throw() {
 	_compt++;
  	return *this;
  }
   
  /* Operador de postincrement. 
  Incrementa en 1 el número de vegades que s'ha trucat al telèfon i
  retorna una còpia d'aquest telèfon sense incrementar.
  Cost Temporal : O(1) */
  phone phone::operator++(int) throw() {
  	phone temp = *this;
  	_compt++;
  	return temp;
  }
  
  
  /* Operadors de comparació.  L'operador > retorna cert, si i només si, el
  telèfon sobre el que s'aplica el mètode és més freqüent que el
  telèfon T, o a igual freqüència, el nom associat al telèfon és
  major en ordre lexicogràfic que el nom associat a T. 
  La resta d'operadors es defineixen consistentment respecte a >. 
  Cost Temporal : O(1) */
  bool phone::operator==(const phone& T) const throw() {
  	return _compt == T._compt && _name == T._name;
  }
  
  //Cost Temporal : O(1)
  bool phone::operator!=(const phone& T) const throw() {
	return !(*this == T);
  }
  
  //Cost Temporal : O(1)
  bool phone::operator<(const phone& T) const throw() {
 	if (_compt < T._compt) return true;
    if (_compt > T._compt) return false;
    return _name < T._name;
  }
  
  //Cost Temporal : O(1)
  bool phone::operator>(const phone& T) const throw() {
  	if(_compt > T._compt) return true;
  	if(_compt < T._compt) return false;
  	return _name > T._name;
  	
  }
  
  //Cost Temporal : O(1)
  bool phone::operator<=(const phone& T) const throw() {
  	return !(*this > T);
  }
  
  //Cost Temporal : O(1)
  bool phone::operator>=(const phone& T) const throw() {
  	return !(*this < T);
  }
  
