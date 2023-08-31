import React from 'react'
import { useState } from 'react';
import { Button } from '@mui/material';
import '../css/Controller.css'
import { useForm } from "react-hook-form";

function Controller() {
   const { register, formState: { errors }, handleSubmit, setError, clearErrors, watch } = useForm();
   const motor_value = watch("motor", 1); // Default value is 1
   
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
            <label style={{ margin: '10px auto' }}>Enter Motor Number (1-3)</label>
            <select {...register("motor")} style={{ width: '100%', height: '22px' }}>
               {[["joint 1", 1], ["joint 2", 2], ["joint 3", 3], ["gripper", 4]].map(([value, index]) => (
                  <option value={index}>{value}</option>
               ))}
            </select>
         </span>
         {(motor_value != 4) ?
         (
         <>
         <span className='form-group'>
            <label style={{ margin: '10px auto' }}>Enter Motor Speed (0-10)</label>
            <input type="number" {...register("speed", { required: false, min: 0, max: 10 })} />

            {errors.speed && <p role="alert" style={{ color: 'red', margin: 'none' }}>{errors.speed.type}</p>}
         </span>
         <span className='form-group'>
            <label style={{ margin: '10px auto' }}>Enter Motor Angle (0-180)</label>
            <input type="number" {...register("angle", { required: false, min: 0, max: 180 })} />

            {errors.angle && <p role="alert" style={{ color: 'red', margin: 'none' }}>{errors.angle.type}</p>}
         </span>
         {errors.custom && <p role="alert" style={{ color: 'red', margin: 'none' }}>{errors.custom.message}</p>}
         </>
         )
         :
         (
         <>
         <span className='form-group'>
            <label style={{ margin: '10px auto' }}>Close or Open?</label>
            <select {...register("gripper_pos")} style={{ width: '100%', height: '30px' }}>
               {[["open", 1], ["close", 2]].map(([value, index]) => (
                  <option value={index}>{value}</option>
               ))}
            </select>
         </span>
         </>
         )
         }

         <Button variant="contained" type='submit' style={{ margin: '10px auto' }} onClick={() => {
            clearErrors("custom");
         }}><b>Submit Command</b></Button>
      </form>
   );
}

export default Controller