import React from 'react'
import { Button } from '@mui/material';
import '../css/Controller.css'
import { useForm } from "react-hook-form";

function Controller() {
   const { register, formState: { errors }, handleSubmit, setError, clearErrors } = useForm();
   const onSubmit = (data) => {
      if (!data.speed && !data.angle) {
         // Set custom error message
         setError("custom", {
            type: "manual",
            message: "Either Motor Speed or Motor Angle must be provided"
         });
         return;
      }
      // send axios post call
      
   };


   return (
      <form onSubmit={handleSubmit(onSubmit)} style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', width: '100%', height: '80vh' }}>
         <h1>Controller</h1>
         <span className='form-group'>
            <label style={{ margin: '10px auto' }}>Enter Motor Number (1-4)</label>
            <input type="number" {...register("motor", { required: true, min: 1, max: 4 })} />
            {errors.motor && <p role="alert" style={{ color: 'red', margin: 'none' }}>{errors.motor.type}</p>}
         </span>
         <span className='form-group'>
            <label style={{ margin: '10px auto' }}>Enter Motor Speed (0-10)</label>
            <input type="number" {...register("speed", { required: false, min: 0, max: 10 })} />
            {errors.speed && <p role="alert" style={{ color: 'red', margin: 'none' }}>{errors.speed.type}</p>}
         </span>
         <span className='form-group'>
            <label style={{ margin: '10px auto' }}>Enter Motor Angle (-360-360)</label>
            <input type="number" {...register("angle", { required: false, min: -360, max: 360 })} />
            {errors.angle && <p role="alert" style={{ color: 'red', margin: 'none' }}>{errors.angle.type}</p>}
         </span>
         {errors.custom && <p role="alert" style={{ color: 'red', margin: 'none' }}>{errors.custom.message}</p>}
         <Button variant="contained" type='submit' style={{ margin: '10px auto' }} onClick={()=>{
            clearErrors("custom");
         }}><b>Submit Command</b></Button>
      </form>
   );
}

export default Controller