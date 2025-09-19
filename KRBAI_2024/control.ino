void PID_pitch(float pitch_kp, float pitch_ki, float pitch_kd, float pitch)
{
//  pi_Setpoint = 2;  //cmps12
  pi_Setpoint = 0;    //BNO055
  pi_Input =  pitch;
  pitch_PID.SetTunings(pitch_kp,pitch_ki,pitch_kd);
  pitch_PID.Compute();
}

void PID_Depth(float depth_kp, float depth_ki, float depth_kd, float depth)
{
  // reset_level = nilai presure permukaan
  //de_Setpoint = 1015;
  de_Setpoint = reset_level + set_level;
  de_Input =  depth;
  depth_PID.SetTunings(depth_kp,depth_ki,depth_kd);
  depth_PID.Compute();
  // Serial.print("set_level= ");   Serial.print(de_Setpoint);
  // Serial.print(", de_Input= ");   Serial.println(depth);
}

 void PID_Heading(float head_kp, float head_ki, float head_kd, float heading)
 {
   he_Setpoint = set_head;
   he_Input =  heading;
   head_PID.SetTunings(head_kp,head_ki,head_kd);
   head_PID.Compute();
 }

 void pitch_depth_head(){
  PID_pitch(pitch_kp, pitch_ki, pitch_kd, Pitch);
  PID_Depth(depth_kp, depth_ki, depth_kd, avg);
  PID_Heading(head_kp, head_ki, head_kd, Yaw);
  if(pitch_status){
    if(pi_Output>0){
      thrus_pi.writeMicroseconds(1500 + abs(pi_Output));
    }
    else if (pi_Output<0){
      thrus_pi.writeMicroseconds(1500 - abs(pi_Output));
    }
  }
  if(depth_status){
    if(de_Output<0){
      thrus_ka.writeMicroseconds(1500 - abs(de_Output));
      thrus_ki.writeMicroseconds(1500 - abs(de_Output));
    }
    else if (de_Output>0){
      thrus_ka.writeMicroseconds(1500 + abs(de_Output));
      thrus_ki.writeMicroseconds(1500 + abs(de_Output));
    } 
  }
  
}

void geserkanan_head(){
  if(head_status){
    gap_atas = set_head + head_tolerance;
    gap_bawah = set_head - head_tolerance;
    if(Yaw < gap_bawah){ 
      rr_PID(abs(he_Output));
    }
    else if (Yaw > gap_atas){
      lr_PID(abs(he_Output));
    }
    else if (Yaw<gap_atas && Yaw>gap_bawah){
      geser_kanan();
    }
  }
}
void geserkiri_head(){
  if(head_status){
    gap_atas = set_head + head_tolerance;
    gap_bawah = set_head - head_tolerance;
    if(Yaw < gap_bawah){ 
      rr_PID(abs(he_Output));
    }
    else if (Yaw > gap_atas){
      lr_PID(abs(he_Output));
    }
    else if (Yaw<gap_atas && Yaw>gap_bawah){
      geser_kiri();
    }
  }
}

