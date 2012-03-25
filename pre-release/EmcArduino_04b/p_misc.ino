
void jog(long x, long y, long z, long a) {
  // do some tests here to validate x, y, and z.
  pos_x=x;
  pos_y=y;
  pos_z=z;
  pos_a=a;
 //         moveTo('X',pos_x);
 //         moveTo('Y',pos_y);
 //         moveTo('Z',pos_z);

}


long findDiff(long a, long b)
{ // find distance between to integers.
  long c;
  if(a>0 && b>0){if(a<b){c=b-a;}else{c=a-b;}}
  if(a<0 && b<0){if(a<b){c=b-a;}else{c=a-b;}}
  if(a<0 && b>0){c=b+(a*-1);}
  if(a>0 && b<0){c=a+(b*-1);}
  if(c<0){c=c*-1;}
  return c; // a positive integer.
}

