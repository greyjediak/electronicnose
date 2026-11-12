#include <>


void uiStart(){

}

void uiBuzz(){
    tone(buzzWire, 60);
    delay(3000);
    noTone(buzzWire);
}