// Importing files from Material-UI
import React from 'react';
import { Toolbar, AppBar, Typography,IconButton} from '@mui/material';
import Link from '@mui/material/Link';
import { makeStyles } from '@mui/styles';

// Using Inline Styling
const useStyles = makeStyles({
   root: {
      flexGrow: 1,
   },
   menuButton: {
      marginRight: "20px",
   },
});

// Exporting Default Navbar to the App.js File
export default function Navbar() {
   const classes = useStyles();

   return (
      <div className={classes.root}>
         <AppBar position="static">
            <Toolbar variant="dense">
               <IconButton edge="start"
                  className={classes.menuButton}
                  color="inherit" aria-label="menu">
               </IconButton>
               <Typography variant="h6" color="inherit">
               <Link color="inherit" href="http://127.0.0.1:3000/" marginRight={10}>
                  Home
               </Link>
               </Typography>
               <Typography variant="h6" color="inherit">
               <Link color="inherit" href="http://000000000:3000/controller">
                  Controller
               </Link>
               </Typography>
            </Toolbar>
         </AppBar>
      </div>
   );
}