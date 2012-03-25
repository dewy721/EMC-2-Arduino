
int processLimits(char Axis, float Position)
{
  int limit=-1;
  if(Axis=='X')
  {
    if(currentX < xMinLimit)
    {
      limit=0; // xMinLimit reached.
    }else{
      if(currentX > xMaxLimit)
      {
        limit=2; // xMaxLimit reached.
      }else{
        limit=1; // niether xMinLimit nor xMaxLimit reached.
      }
    }
    if(limit != xLimitOld)
    {
      xLimitOld=limit;
      Serial.print("x");
      Serial.print(limit);
    }
  }
  if(Axis=='Y')
  {
    if(currentY < yMinLimit)
    {
      limit=0; // yMinLimit reached.
    }else{
      if(currentY > yMaxLimit)
      {
        limit=2; // yMaxLimit reached.
      }else{
        limit=1; // niether yMinLimit nor yMaxLimit reached.
      }
    }
    if(limit != yLimitOld)
    {
      yLimitOld=limit;
      Serial.print("y");
      Serial.print(limit);
    }
  }
  if(Axis=='Z')
  {
    if(currentZ < zMinLimit)
    {
      limit=0; // zMinLimit reached.
    }else{
      if(currentZ > zMaxLimit)
      {
        limit=2; // zMaxLimit reached.
      }else{
        limit=1; // niether zMinLimit nor xMaxLimit reached.
      }
    }
    if(limit != zLimitOld)
    {
      zLimitOld=limit;
      Serial.print("z");
      Serial.print(limit);
    }
  }
  return limit;
}

