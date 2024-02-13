#include "dialog.hpp"

/* Retorna en el vector answers els resultats obtinguts al processar
els successius caràcters de l'string input, i en numtelf retorna 
el número de telèfon corresponent a l'últim nom obtingut o un 0 si
no existeix aquest nom de telèfon. Si durant el processament de la
seqüència de tecles representada en input es produís un error
llavors a answers es registra el missatge d'error associat a
l'excepció, numtelf és un 0 i finalitza el procés. 
Cost Temporal: O(n) on n és la longitud del string input*/
void dialog::dialog(easy_dial& easy, const string& input, 
		  vector<string>& answers, nat& numtelf) throw(){
		
	numtelf = 0;
		
	answers.push_back( easy.inici() );
			  
	for(int i = 0; i < input.size(); i++) {
	
		if( input[i] == phone::ENDCHAR ) {
	
			
			
			try {
		
				answers.push_back( easy.seguent(phone::ENDPREF) );
				
			} catch(error e) {
				
				
			}
	
		} else if( input[i] == phone::DELETECHAR ) {
			
			try {
		
				answers.push_back( easy.anterior() );
				
			} catch(error e) {
				
				
			}
			
			
		} else {
			
						
			try {
		
				answers.push_back( easy.seguent(input[i]) );
				
			} catch(error e) {
				
				
			}
			
		}
		
	}
	
	try {
	
		numtelf = easy.num_telf();
		
	}catch(error e) {
		
		answers.push_back( e.mensaje() );
		
	}
			  
}
