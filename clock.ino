int mode=0;
int st=0;
int timer=0;
int hour=0;
int year=24;
int month=1;
int day=1;
int min=0;
int update=0;
int show=0;
int sec=0;
int light=0;
void ledef(int num,int dig);
const int pinState[10][7] = {
 //2, 3, 4, 5, 6, 7, 8
 //7,10,11, 5, 4, 2, 1
 //B, F, A, G, C, D, E
  {0, 0, 0, 1, 0, 0, 0}, // 0
  {0, 1, 1, 1, 0, 1, 1}, // 1
  {0, 1, 0, 0, 1, 0, 0}, // 2
  {0, 1, 0, 0, 0, 0, 1}, // 3
  {0, 0, 1, 0, 0, 1, 1}, // 4
  {1, 0, 0, 0, 0, 0, 1}, // 5
  {1, 0, 0, 0, 0, 0, 0}, // 6
  {0, 1, 0, 1, 0, 1, 1}, // 7
  {0, 0, 0, 0, 0, 0, 0}, // 8
  {0, 0, 0, 0, 0, 0, 1}  // 9
};
void setup() {
  // put your setup code here, to run once:
  for(int i=14;i<17;i++) {
    pinMode(i,INPUT_PULLUP);
  }
  for(int j=2;j<13;j++) {
    pinMode(j,OUTPUT);
  }
  pinMode(13,OUTPUT);
  pinMode(17,INPUT);
  Serial.begin(115200);
}

void ledef(int num,int dig) {
  if(dig==0 and show==0){
    digitalWrite(13,sec);
    if(st>=1 and st<=2){
      digitalWrite(13,0);
    }
  }else{
    digitalWrite(13,1);
  }
  for(int i=9;i<=12;i++) {
    digitalWrite(i,0);
  }
  for(int i=0;i<=6;i++) {
      digitalWrite(i+2,pinState[num][i]);
  }
  if(st==1 or st==3){
    if(dig==0 or dig==1){
      digitalWrite(dig+9,sec);
    }else{
      digitalWrite(dig+9,1);
    }
  }else if(st==2 or st==4){
    if(dig==2 or dig==3){
      digitalWrite(dig+9,sec);
    }else{
      digitalWrite(dig+9,1);
    }
  }else if(st==5){
    digitalWrite(dig+9,sec);
  }else if(light>=391){
    digitalWrite(dig+9,0);
  }else{
    digitalWrite(dig+9,1);
  }
  delay(5);
  timer+=1;
  if(timer%98==0){
    sec+=1;
    if(sec>1){
      sec=0;
    }
  }
  if(analogRead(17)<=100 and light<=392){
    light+=1;
  }if(analogRead(17)>=101 or mode>=5 or st>0){
    light=0;
  }
}
void time(){
  if(min>59){
    min=0;
    if(st==0){
      hour+=1;
    }
  }
  if(hour>23){
    hour=0;
    if(st==0){
      day+=1;
    }
  }
  if(hour<0){
    hour+=24;
  }
  if(min<0){
    min+=60;
  }
  if(timer>784){//<<<===
    if(st==0){
      min+=1;
    }
    timer=0;
  }
  //===mmdd====================
  if(day<1){
    if(month==2 and year%4!=0){
      day+=28;
    }
    if(month==2 and year%4==0){
      day+=29;
    }
    if(month==1 or month==3 or month==5 or month==7 or month==8 or month==10 or month==12){
      day+=31;
    }
    if(month==4 or month==6 or month==9 or month==11){
      day+=30;
    }
  }
  if(month<1){
    month+=12;
  }
  if(month>12){
    if(st==0){
      year+=1;
    }
    month=1;
  }
  if(day>28){
    if(month==2 and year%4!=0){
      if(st==0){
        month+=1;
      }
      day=1;
    }
    if(day>29 and month==2 and year%4==0){
      if(st==0){
        month+=1;
      }
      day=1;
    }
    if(month==1 or month==3 or month==5 or month==7 or month==8 or month==10 or month==12){
      if(day>31){
        if(st==0){
          month+=1;
        }
        day=1;
      }
    }
    if(month==4 or month==6 or month==9 or month==11){
      if(day>30){
        if(st==0){
          month+=1;
        }
        day=1;
      }
    }
  }
  //===year====================
  if(year<20){
    year=year+80;
  }
  if(year>99){
    year=20;
  }
  if(show==0){
    int m=min/10;
    int n=min%10;
    int o=hour/10;
    int p=hour%10;
    ledef(o,1);
    ledef(p,0);
    ledef(m,3); 
    ledef(n,2);

  }
  if(show==1){
    int m=day/10;
    int n=day%10;
    int o=month/10;
    int p=month%10;
    ledef(o,1);
    ledef(p,0);
    ledef(m,3); 
    ledef(n,2);
  }
  if(show==2){
    int m=year/10;
    int n=year%10;
    int o=2;
    int p=0;
    ledef(o,1);
    ledef(p,0);
    ledef(m,3); 
    ledef(n,2);
  }
}
//===========================
void allnotshow(int d){
  for(int i=9;i<13;i++){
    digitalWrite(i,0);
  }
  delay(d);
}
void mobtn(){
  Serial.println(analogRead(17));
  if (digitalRead(14)==0) {
    mode+=1;
  }
}
void loop() {
  time();
  mobtn();
  if(digitalRead(14)==1){
    if(mode>5 and mode<20){
      show+=1;
      if(show>2){
        show=0;
      }
      allnotshow(500);
      timer+=100;
      mode=0;
    }
    if(mode>150){
      st=1;
      allnotshow(500);
      mode=0;
    }
    mode=0;
  }
  //===========================
  while(st>0){
    while(st==1){//hour
      show=0;
      time();
      mobtn();
      if(digitalRead(15)==0){
        hour+=1;
        allnotshow(200);
      }
      if(digitalRead(16)==0){
        hour-=1;
        allnotshow(200);
      }
      if(mode>5){
        st=2;
        allnotshow(200);
        mode=0;
      }
    }
    while(st==2){//min
      show=0;
      time();
      mobtn();
      if(digitalRead(15)==0){
        min+=1;
        allnotshow(200);
      }
      if(digitalRead(16)==0){
        min-=1;
        allnotshow(200);
      }
      if (mode>5) {
        st=3;
        allnotshow(200);
        mode=0;
      }
    }
    while(st==3){//month
      show=1;
      time();
      mobtn();
      if(digitalRead(15)==0){
        month+=1;
        allnotshow(200);
      }
      if(digitalRead(16)==0){
        month-=1;
        allnotshow(200);
      }
      if (mode>5) {
        st=4;
        allnotshow(200);
        mode=0;
      }
    }
    while(st==4){//day
      show=1;
      time();
      mobtn();
      if(digitalRead(15)==0){
        day+=1;
        allnotshow(200);
      }
      if(digitalRead(16)==0){
        day-=1;
        allnotshow(200);
      }
      if (mode>5) {
        st=5;
        allnotshow(200);
        mode=0;
      }
    }
    while(st==5){//year
      show=2;
      time();
      mobtn();
      if(digitalRead(15)==0){
        year+=1;
        allnotshow(200);
      }
      if(digitalRead(16)==0){
      year-=1;
        allnotshow(200);
      }
      if (mode>5) {
        st=0;
        allnotshow(200);
        mode=0;
        show=0;
      }
    }
  }
}
