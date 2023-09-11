import * as React from 'react'
import { useState, useEffect } from 'react';
import { Button } from '@mui/material';
import Snackbar from '@mui/material/Snackbar';
import '../css/Controller.css'
import axios from 'axios';
import { useForm } from "react-hook-form";
import MuiAlert from '@mui/material/Alert';

const Alert = React.forwardRef(function Alert(props, ref) {
   return <MuiAlert elevation={6} ref={ref} variant="filled" {...props} />;
});

function Controller() {
   const { register, formState: { errors }, handleSubmit, setError, clearErrors, watch } = useForm();
   const motor_value = watch("motor", 1); // Default value is 1
   const [open, setOpen] = useState("");
   const [angle_array, setAngleArray] = useState([null, null, null]);

   function getAngleArray() {
      axios.get('http://192.168.1.2:8080/get_joint_angles')
         .then(response => {
            setAngleArray(response.data.split(',').map(Number));
         })
         .catch(error => {
            setAngleArray([null, null, null]);
            console.log(error);
         });
   }

   function handleClose() {
      setOpen("");
   }

   const onSubmit = async (data) => {
      // send axios post call
      let response;
      if (data.motor != 4) {
         if (!data.speed && !data.angle) {
            // Set custom error message
            setError("custom", {
               type: "manual",
               message: "Either Motor Speed or Motor Angle must be provided"
            });
            return;
         }
         response = await axios.post('http://192.168.1.2:8080/move_joint', data);
      } else {
         response = await axios.post('http://192.168.1.2:8080/close_open_gripper', data);
      }
      if (response.status == 200) {
         setOpen("success");
      } else if (response.status == 400) {
         setOpen("Enter Correct Parameters");
      } else {
         setOpen(`${response.status} Error`);
      }
   };

   // use useEffect to get call server to populate the angle_array
   useEffect(() => {
      getAngleArray();
   }, []);

   return (
      <>
         <div className="main_container" style={{ display: 'flex', flexDirection: 'row', alignItems: 'center', justifyContent: 'center', height: '80vh', width: '100%' }}>
            <div className="container" style={{ display: 'flex', flexDirection: 'column', justifyContent: 'center', alignItems: 'center', width: '20%', border: '2px solid white' }}>
               <h2>Robot Joint Positions</h2>
               {(angle_array[0] && angle_array[1] && angle_array[2]) ? <div style={{ display: 'flex', flexDirection: 'column', justifyContent: 'center', alignItems: 'center', padding: '10px' }}>
                  <span style={{ marginBottom: '10px' }}>Joint 1 Position: {angle_array[0]}</span>
                  <span style={{ marginBottom: '10px' }}>Joint 2 Position: {angle_array[1]}</span>
                  <span style={{ marginBottom: '10px' }}>Joint 3 Position: {angle_array[2]}</span>
               </div> :
                  <h4 style={{ color: 'red' }}>Robot is not connected</h4>}
            </div>
            <form onSubmit={handleSubmit(onSubmit)} style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', width: '70%' }}>
               <h1>Controller</h1>
               <span className='form-group'>
                  <label style={{ margin: '10px auto' }}>Choose Motor</label>
                  <select {...register("motor")} style={{ width: '100%', height: '22px' }}>
                     {[["joint 1", 1], ["joint 2", 2], ["joint 3", 3], ["gripper", 4]].map(([value, index]) => (
                        <option value={index} key={index}>{value}</option>
                     ))}
                  </select>
               </span>
               {(motor_value != 4) ?
                  (
                     <>
                        <span className='form-group'>
                           <label style={{ margin: '10px auto' }}>Enter Motor Speed (1-10)</label>
                           <input type="number" {...register("speed", { required: false, min: 1, max: 10 })} />

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
                           <select {...register("gripper_pos")} style={{ width: '100%', height: '22px' }}>
                              {[["open", 1], ["close", 2]].map(([value, index]) => (
                                 <option value={index} key={index}>{value}</option>
                              ))}
                           </select>
                        </span>
                     </>
                  )
               }

               <Button variant="contained" type='submit' style={{ margin: '10px auto' }} onClick={() => {
                  clearErrors("custom");
               }}><b>Submit Command</b></Button>
               <Snackbar open={open} autoHideDuration={6000} onClose={handleClose}>
                  <Alert onClose={handleClose} severity={open} sx={{ width: '100%' }}>
                     {(open == "success") ? "The command has been successfully sent!" : open}
                  </Alert>
               </Snackbar>
            </form>
         </div>
      </>
   );
}

export default Controller