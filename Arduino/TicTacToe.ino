/* Curso: 2510 - ENGENHARIA DE COMPUTACAO                        ||
*  Disciplina: 10514 - T1 - Laboratório de Microprocessadores    ||
*  Professor: GCastilhos                                         ||
*  Ano/Período: 2013/1                                           ||
*----------------------------------------------------------------||
*                                                                ||
*=================================================================|
******************************************************************/

char talvez[8];
char possivel[8];
char possicao[10];
char casoNada[10];

int numJog   = 0;
int quemJoga = 0;
int adv      = 0;
int cor      = 0;
int jogada   = 0;

boolean fim     = false;
boolean opMac   = false;
boolean proximo = false;


void setup(){
  analogRead( A0 );
  analogRead( A1 );
  
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A5, OUTPUT);
  
  fim      = false;
  quemJoga = 0;
  numJog   = 0;
}

void loop(){
      SinalLed(5, 150);
	  
      for (int x=1; x<10; x++){
        possicao[x]='V';
      }
      
      for (int x=0; x<8; x++){
            possivel[x] = 's';
            talvez[x]   = 's';
            casoNada[x] = 's';
        }
        
      //Loop to get the number of players
      while(digitalRead(10)==LOW  && digitalRead(11)==LOW){
        delay(1);  
      }
      while(digitalRead(10)==HIGH){
        numJog=1;
        opMac=true;
      
      }
      while(digitalRead(11)==HIGH){
        numJog=2;
        opMac=false;
      }
      
	  if(numJog == 1){
		digitalWrite(12,HIGH);
		digitalWrite(13,LOW);
      } else {
        digitalWrite(12,HIGH);
        digitalWrite(13,HIGH);
      }
      delay(500);  
      //---------------------------------------------------------------
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);
      
	  //Loop to choose who start
      SinalLed(3,150);
      while(digitalRead(10)==LOW  && digitalRead(11)==LOW){
        delay(1);  
      }
      while(digitalRead(10)==HIGH){
        quemJoga=1;
      }
      while(digitalRead(11)==HIGH){
        quemJoga=2;
      }
      
	  //Start the game
      while(fim==false){
        
		//player one
		if(quemJoga==1){
			  int entrada;
			  boolean estado = false;
			  digitalWrite(12,HIGH);
			  digitalWrite(13,LOW);
			  cor=0;
			  digitalWrite(0,cor);
			  while(estado==false){
				  entrada=LeAnalogico();
				  if (possicao[entrada]=='V'){
				  //if the place is empty
					  digitalWrite(entrada,HIGH);
					  estado=digitalRead(10);
				 }
				  for (int x=1;x<10;x++){
						  if(possicao[x]=='V' && x != entrada){
							digitalWrite(x,LOW);
						  }
					  }
			 }
			 possicao[entrada]='H';
			 quemJoga=2;
			}
			fim = TestaFim();
			//player two
			if(fim==false && quemJoga==2 && opMac==false){
				  int entrada;
				  boolean estado = false;
				  digitalWrite(13,HIGH);
				  digitalWrite(12,LOW);
				  cor=1;
				  digitalWrite(0,cor);
				  while(estado==false){
					  entrada=LeAnalogico();
					  if (possicao[entrada]=='V'){
					  //if the place is empty
						  digitalWrite(entrada,HIGH);
						  estado=digitalRead(11);
					 }
					  for (int x=1;x<10;x++){
							  if(possicao[x]=='V' && x != entrada){
								digitalWrite(x,LOW);
							  }
						  }
				 }
				 possicao[entrada]='M';
				 quemJoga=1;
			} 
			fim=TestaFim();
			
			//if play with the computer
			if(fim==false && quemJoga==2 && opMac==true){
			  cor=1;
			  digitalWrite(0,cor);
			  delay(500);
			  int possoGanhar=0;
			  int possoPerder=0;
			  int qualquer=0;
			  int proxPossicao=0;
			  
			  possoPerder = PossoPerder();
			  if(possoPerder!=0){
				proxPossicao=possoPerder;
			  }
			  
			  possoGanhar = PossoGanhar();
			  if(possoGanhar!=0 && proxPossicao==0){
					proxPossicao=possoGanhar;
			  }
			  
			  qualquer=QualquerCoisa();
			  if(qualquer!=0 && proxPossicao==0){
				   proxPossicao=qualquer;
			  }
				digitalWrite(proxPossicao,HIGH);
				possicao[proxPossicao]='M';
				quemJoga=1;
			}
				
		   fim=TestaFim();
	 }
			
			
			

	//endgame
	delay(500);
	SinalLed(15,90);

    delay(1000);  
}

void SinalLed(int rep,int dela){
    for(int x=0;x<rep;x++){
    digitalWrite(12,HIGH);
    digitalWrite(13,HIGH);
    delay(dela);
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    delay(dela);
  }
}
int LeAnalogico(){
  //A0 for column
  //A1 for line
  int sensorV = analogRead(A0);
  int sensorH = analogRead(A1);
  sensorV = map(sensorV,0,1023,0,2);
  sensorH = map(sensorH,0,1023,1,3);
  int result = sensorV*3+sensorH;
  return result;
}

//checks
boolean TestaFim(){
  
  int vitoria=TestaVitoria();
  if(vitoria==1){
    ligaTodos(10,0,100);
    return true;
  }
  else if(vitoria==2){
    ligaTodos(10,1,100);
    return true;
  }  
  
  else if(TestaEmpate()){
      ligaTodos(10,2,100);
      return true;
  }
  return false;
}

int TestaVitoria(){
  int retorno=0;
  //Diagonals
  if(possicao[1]==possicao[5] && possicao[1]==possicao[9] && possicao[1]=='H'){
        retorno=1;
    }
   if(possicao[1]==possicao[5] && possicao[1]==possicao[9] && possicao[1]=='M'){
        retorno=2;
    }
   
   else if(possicao[7]==possicao[5] && possicao[7]==possicao[3] && possicao[7]=='H'){
        retorno=1;
    }
   else if(possicao[7]==possicao[5] && possicao[7]==possicao[3] && possicao[7]=='M'){
        retorno=2;
    }
   
   //lines
     if(possicao[1]==possicao[2] && possicao[1]==possicao[3] && possicao[1]=='H'){
           retorno=1;
     }
     if(possicao[4]==possicao[5] && possicao[4]==possicao[6] && possicao[4]=='H'){
           retorno=1;
     }
     if(possicao[7]==possicao[8] && possicao[7]==possicao[9] && possicao[7]=='H'){
           retorno=1;
     }
     if(possicao[1]==possicao[2] && possicao[1]==possicao[3] && possicao[1]=='M'){
           retorno=2;
     }
     if(possicao[4]==possicao[5] && possicao[4]==possicao[6] && possicao[4]=='M'){
           retorno=2;
     }
     if(possicao[7]==possicao[8] && possicao[7]==possicao[9] && possicao[7]=='M'){
           retorno=2;
     }
	 
     //columns
     if(possicao[1]==possicao[4] && possicao[1]==possicao[7] && possicao[1]=='H'){
           retorno=1;
     }
     if(possicao[2]==possicao[5] && possicao[2]==possicao[8] && possicao[2]=='H'){
           retorno=1;
     }
     if(possicao[3]==possicao[6] && possicao[3]==possicao[9] && possicao[3]=='H'){
           retorno=1;
     }
     if(possicao[1]==possicao[4] && possicao[1]==possicao[7] && possicao[1]=='M'){
           retorno=2;
     }
     if(possicao[2]==possicao[5] && possicao[2]==possicao[8] && possicao[2]=='M'){
           retorno=2;
     }
     if(possicao[3]==possicao[6] && possicao[3]==possicao[9] && possicao[3]=='M'){
           retorno=2;
     }
      
     return retorno;
}
  
boolean TestaEmpate(){
  int contador=0;
    for (int x=1;x<10;x++){
      if(possicao[x]!='V'){
          contador++;
      }
    }
    if (contador==9){
      return true;
    }
    else{
      return false;
    }
}
void ligaTodos(int vezes,int cor,int dela){
  
  int alterna=1;
    for (int x = 0;x<vezes;x++){
        if(cor==2){
          alterna=(alterna+1)%2;
          digitalWrite(0,alterna);
        }
        else{
          digitalWrite(0,cor);
        }
        
        digitalWrite(1,HIGH);
        digitalWrite(2,HIGH);
        digitalWrite(3,HIGH);
        digitalWrite(4,HIGH);
        digitalWrite(5,HIGH);
        digitalWrite(6,HIGH);
        digitalWrite(7,HIGH);
        digitalWrite(8,HIGH);
        digitalWrite(9,HIGH);
        delay(dela);
        digitalWrite(1,LOW);
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);
        digitalWrite(9,LOW);
        delay(dela);
   }
}

int PossoPerder(){
  int percoaqui=0;
	//for lines
	if (talvez[0]=='s'){
		if(possicao[1]=='M'&& possicao[2]=='M' ||possicao[1]=='M'&& possicao[3]=='M' || possicao[2]=='M'&& possicao[3]=='M'){
			//________________________existem duas peças da pessoa na primeira linha
			if(possicao[1]=='H'||possicao[2]=='H'||possicao[3]=='H'){
				talvez[0]='n';
			}
			else{
				if(possicao[1]=='V'){
					percoaqui=1;
				}
				else if(possicao[2]=='V'){
					percoaqui=2;
				}
				else if(possicao[3]=='V'){
					percoaqui=3;
				}
			}
		}
	}
             
	if (talvez[1]=='s'){
		if(possicao[4]=='M'&& possicao[5]=='M' ||possicao[4]=='M'&& possicao[6]=='M' || possicao[5]=='M'&& possicao[6]=='M'){
			if(possicao[4]=='H'||possicao[5]=='H'||possicao[6]=='H'){
				talvez[1]='n';
			}
			else{
				if(possicao[4]=='V'){
					percoaqui=4;
				}
				else if(possicao[5]=='V'){
					percoaqui=5;
				}
				else if(possicao[6]=='V'){
					percoaqui=6;
				}
			}
		}
	}
	if (talvez[2]=='s'){
		if(possicao[7]=='M'&& possicao[8]=='M' ||possicao[7]=='M'&& possicao[9]=='M' || possicao[8]=='M'&& possicao[9]=='M'){
			if(possicao[7]=='H'||possicao[8]=='H'||possicao[9]=='H'){
				talvez[2]='n';
			}
			else{
				if(possicao[7]=='V'){
					percoaqui=7;
				}
				else if(possicao[8]=='V'){
					percoaqui=8;
				}
				else if(possicao[9]=='V'){
					percoaqui=9;
				}
			}
		}
	}
     
     //for columns
    if (talvez[3]=='s'){
		if(possicao[1]=='M'&& possicao[4]=='M' || possicao[1]=='M'&& possicao[7]=='M' || possicao[4]=='M'&& possicao[7]=='M'){
			if(possicao[1]=='H'||possicao[4]=='H' ||possicao[7]=='H'){
				talvez[3]='n';
			}
			else{
				if(possicao[1]=='V'){
					percoaqui=1;
				}
				else if(possicao[4]=='V'){
					percoaqui=4;
				}
				else if(possicao[7]=='V'){
					percoaqui=7;
				}
			}
		}
	}
   if (talvez[4]=='s'){
		if(possicao[2]=='M'&& possicao[5]=='M' || possicao[2]=='M'&& possicao[8]=='M' || possicao[5]=='M'&& possicao[8]=='M'){
			if(possicao[2]=='H'||possicao[5]=='H' ||possicao[8]=='H'){
				talvez[4]='n';
			}
			else{
				if(possicao[2]=='V'){
					percoaqui=2;
				}
				else if(possicao[5]=='V'){
					percoaqui=5;
				}
				else if(possicao[8]=='V'){
					percoaqui=8;
				}
			}
		}
	}
	if (talvez[5]=='s'){
		if(possicao[3]=='M'&& possicao[6]=='M' || possicao[3]=='M'&& possicao[9]=='M' || possicao[6]=='M'&& possicao[9]=='M'){
			if(possicao[3]=='H'||possicao[6]=='H' ||possicao[9]=='H'){
				talvez[5]='n';
			}
			else{
				if(possicao[3]=='V'){
					percoaqui=3;
				}
				else if(possicao[6]=='V'){
					percoaqui=6;
				}
				else if(possicao[9]=='V'){
					percoaqui=9;
				}
			}
		}
	}
	
	//for diagonals
	if (talvez[6]=='s'){
		if(possicao[1]=='M'&& possicao[5]=='M' || possicao[1]=='M'&& possicao[9]=='M' || possicao[5]=='M'&& possicao[9]=='M'){
			if(possicao[1]=='H'||possicao[5]=='H'||possicao[9]=='H'){
				talvez[6]='n';
			}
			else{
				if(possicao[1]=='V'){
					percoaqui=1;
				}
                    else if(possicao[5]=='V'){
                        percoaqui=5;
                    }
                    else if(possicao[9]=='V'){
                        percoaqui=9;
                    }
                }
            }
        }
		
        if (talvez[7]=='s'){
            if(possicao[3]=='M' && possicao[5]=='M' || possicao[3]=='M'&& possicao[7]=='M' || possicao[5]=='M'&& possicao[7]=='M'){
                if(possicao[3]=='H'||possicao[5]=='H'||possicao[7]=='H'){
                    talvez[7]='n';
                }
                else{
                    if(possicao[3]=='V'){
                        percoaqui=3;
                    }
                    else if(possicao[5]=='V'){
                        percoaqui=5;
                    }
                    else if(possicao[7]=='V'){
                        percoaqui=7;
                    }
                }
            }
        }
    return percoaqui;
 }
  int PossoGanhar(){
  int ganhoaqui=0;
	//for lines
		if (possivel[0]=='s'){
			if(possicao[1]=='H'&& possicao[2]=='H' ||possicao[1]=='H'&& possicao[3]=='H' || possicao[2]=='H'&& possicao[3]=='H'){
				if(possicao[1]=='M'||possicao[2]=='M'||possicao[3]=='M'){
					possivel[0]='n';
				}
				else{
					if(possicao[1]=='V'){
						ganhoaqui=1;
					}
					else if(possicao[2]=='V'){
						ganhoaqui=2;
					}
					else if(possicao[3]=='V'){
						ganhoaqui=3;
					}
				}
			}
		}
		if (possivel[1]=='s'){
			if(possicao[4]=='H'&& possicao[5]=='H' ||possicao[4]=='H'&& possicao[6]=='H' || possicao[5]=='H'&& possicao[6]=='H'){
				if(possicao[4]=='M'||possicao[5]=='M'||possicao[6]=='M'){
					possivel[1]='n';
				}
				else{
					if(possicao[4]=='V'){
						ganhoaqui=4;
					}
					else if(possicao[5]=='V'){
						ganhoaqui=5;
					}
					else if(possicao[6]=='V'){
						ganhoaqui=6;
					}
				}
			}
		}
		if (possivel[2]=='s'){
			if(possicao[7]=='H'&& possicao[8]=='H' ||possicao[7]=='H'&& possicao[9]=='H' || possicao[8]=='H'&& possicao[9]=='H'){
				if(possicao[7]=='M'||possicao[8]=='M'||possicao[9]=='M'){
					possivel[2]='n';
				}
				else{
					if(possicao[7]=='V'){
						ganhoaqui=7;
					}
					else if(possicao[8]=='V'){
						ganhoaqui=8;
					}
					else if(possicao[9]=='V'){
						ganhoaqui=9;
					}
				}
			}
		}
	//for columns
	if (possivel[3]=='s'){
		if(possicao[1]=='H'&& possicao[4]=='H' || possicao[1]=='H'&& possicao[7]=='H' || possicao[4]=='H'&& possicao[7]=='H'){
			if(possicao[1]=='M'||possicao[4]=='M' ||possicao[7]=='M'){
				possivel[3]='n';
			}
			else{
				if(possicao[1]=='V'){
					ganhoaqui=1;
				}
				else if(possicao[4]=='V'){
					ganhoaqui=4;
				}
				else if(possicao[7]=='V'){
					ganhoaqui=7;
				}
			}
		}
	}
   if (possivel[4]=='s'){
		if(possicao[2]=='H'&& possicao[5]=='H' || possicao[2]=='H'&& possicao[8]=='H' || possicao[5]=='H'&& possicao[8]=='H'){
			if(possicao[2]=='M'||possicao[5]=='M' ||possicao[8]=='M'){
				possivel[4]='n';
			}
			else{
				if(possicao[2]=='V'){
					ganhoaqui=2;
				}
				else if(possicao[5]=='V'){
					ganhoaqui=5;
				}
				else if(possicao[8]=='V'){
					ganhoaqui=8;
				}
			}
		}
	}
	if (possivel[5]=='s'){
		if(possicao[3]=='H'&& possicao[6]=='H' || possicao[3]=='H'&& possicao[9]=='H' || possicao[6]=='H'&& possicao[9]=='H'){
			if(possicao[3]=='M'||possicao[6]=='M' ||possicao[9]=='M'){
				possivel[5]='n';
			}
			else{
				if(possicao[3]=='V'){
					ganhoaqui=3;
				}
				else if(possicao[6]=='V'){
					ganhoaqui=6;
				}
				else if(possicao[9]=='V'){
					ganhoaqui=9;
				}
			}
		}
	}
	
	if (possivel[6]=='s'){
		if(possicao[1]=='H'&& possicao[5]=='H' || possicao[1]=='H'&& possicao[9]=='H' || possicao[5]=='H'&& possicao[9]=='H'){
			if(possicao[1]=='M'||possicao[5]=='M'||possicao[9]=='M'){
				possivel[6]='n';
			}
			else{
				if(possicao[1]=='V'){
					ganhoaqui=1;
				}
				else if(possicao[5]=='V'){
					ganhoaqui=5;
				}
				else if(possicao[9]=='V'){
					ganhoaqui=9;
				}
			}
		}
	}
	
	if (possivel[7]=='s'){
		if(possicao[3]=='H' && possicao[5]=='H' || possicao[3]=='H'&& possicao[7]=='H' || possicao[5]=='H'&& possicao[7]=='H'){
			if(possicao[3]=='M'||possicao[5]=='M'||possicao[7]=='M'){
				possivel[7]='n';
			}
			else{
				if(possicao[3]=='V'){
					ganhoaqui=3;
				}
				else if(possicao[5]=='V'){
					ganhoaqui=5;
				}
				else if(possicao[7]=='V'){
					ganhoaqui=7;
				}
			}
		}
	}
	
    return ganhoaqui;
}
int QualquerCoisa(){
  int prox_pos=0;
        int hum=0;
        int mac=0;
        for(int x=1;x<10;x++){
            if(possicao[x]=='H'){hum++;}
            if(possicao[x]=='M'){mac++;}
        }
        
        if(possicao[5]=='V'){
             prox_pos=5;
        }
        if(mac==1 && hum==1){
            if(possicao[1]!='V'){
                prox_pos=9;
            }
            else if(possicao[9]!='V'){
                prox_pos=1;
            }
            else if(possicao[3]!='V'){
                 prox_pos=7;
            }
            else if(possicao[7]!='V'){
                prox_pos=3;
            }
            else if(possicao[4]!='V' || possicao[2]!='V'){
                prox_pos=1;
            }
            else{
                prox_pos=9;
            }
        } else if(possicao[5]=='H' && hum==1){
            if(mac==0){
                prox_pos=1;
            }
            else{
                if(possicao[1]=='M'){
                    prox_pos=9;
                }
                else if(possicao[9]=='M'){
                    prox_pos=1;
                }
                else if(possicao[3]=='M'){
                    prox_pos=7;
                }
                else if(possicao[7]=='M'){
                    prox_pos=3;
                }
            }
        } else if(possicao[1]=='H' && possicao[9]=='H' && possicao[5]=='M'){
            if(possicao[2]!='v' && possicao[4]!='v' && possicao[6]!='v' && possicao[8]!='v'){
                prox_pos=4;
            }
        } else if(possicao[3]=='H' && possicao[7]=='H' && possicao[5]=='M'){
            if(possicao[2]!='v' && possicao[4]!='v' && possicao[6]!='v' && possicao[8]!='v'){
                prox_pos=4;
            }
        } else if(possicao[5]=='H' && possicao[1]=='H' && possicao[9]=='M'||possicao[5]=='H' && possicao[3]=='H'&& possicao[7]=='M'||possicao[5]=='H' && possicao[7]=='H'&& possicao[3]=='M'||possicao[5]=='H' && possicao[9]=='H'&& possicao[1]=='M'){
            if(possicao[1]=='V'){
                prox_pos=1;
            }
            else if(possicao[3]=='V'){
                prox_pos=3;
            }
            else if(possicao[7]=='V'){
                prox_pos=7;
            }
            if(possicao[9]=='V'){
                prox_pos=9;
            }
            
        else if(possicao[3]=='V' || possicao[7]=='V'){
            if(possicao[7]=='V'){
                prox_pos=7;
            }
            else{
                prox_pos=3;
            }
        }
        
        else if(possicao[2]=='V'){
            prox_pos=2;
        }
        else if(possicao[4]=='V'){
            prox_pos=4;
        }
        else if(possicao[6]=='V'){
            prox_pos=6;
        }
        else if(possicao[8]=='V'){
            prox_pos=8;
        }
    }
    
    if(prox_pos==0){
        if(possicao[1]=='V'){
            prox_pos=1;
        }
        else if(possicao[7]=='V'){
            prox_pos=7;
        }
        else if(possicao[3]=='V'){
            prox_pos=3;
        }
        else if(possicao[9]=='V'){
            prox_pos=9;
        }
        else if(possicao[2]=='V'){
            prox_pos=2;
        }
        else if(possicao[4]=='V'){
            prox_pos=4;
        }
        else if(possicao[6]=='V'){
            prox_pos=6;
        }
        else if(possicao[8]=='V'){
            prox_pos=8;
        }
    }
     return prox_pos;
}
