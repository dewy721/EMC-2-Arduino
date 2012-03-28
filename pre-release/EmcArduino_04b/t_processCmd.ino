void processCommand()
{
  float xx=pos_x;
  float yy=pos_y;
  float zz=pos_z;
  float aa=pos_a;

  char *ptr=buffer;
  while(ptr && ptr<buffer+sofar) {
    ptr=strchr(ptr,' ')+1;
    switch(*ptr) {
    case 'x': case 'X': xx=atof(ptr+1); break;
    case 'y': case 'Y': yy=atof(ptr+1); break;
    case 'z': case 'Z': zz=atof(ptr+1); break;
    case 'a': case 'A': aa=atof(ptr+1); break;
    case 's': case 'S': stepMode('ALL', atoi(ptr+1)); break;
    case 'c': ptr=0; sofar=0; buffer[0] = '\0'; Serial.flush(); break;
    default: ptr=0; break;
    }
  }

  jog(xx,yy,zz,aa);
  targetX=pos_x*stepsPerInchX/500000;
  targetY=pos_y*stepsPerInchY/500000;
  targetZ=pos_z*stepsPerInchZ/500000;
  targetA=pos_a*stepsPerInchA/500000;
}



