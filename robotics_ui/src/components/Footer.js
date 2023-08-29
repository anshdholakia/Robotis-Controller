import React from 'react'
import Box from '@mui/material/Box';
import Typography from '@mui/material/Typography';
import Container from '@mui/material/Container';
import Link from '@mui/material/Link';
import Paper from "@mui/material/Paper";

function Copyright() {
   return (
      <Typography variant="body2" color="white">
         {'Copyright © '}
         <Link color="inherit" href="http://127.0.0.1:3000">
            Robotics Controller
         </Link>{' '}
         {new Date().getFullYear()}
         {'.'}
      </Typography>
   );
}

function Footer() {
   return (
      // https://stackoverflow.com/questions/72440972/how-to-push-the-footer-in-mui-at-the-very-bottom-and-make-it-stick
      <Paper sx={{
         marginTop: 'calc(10% + 60px)',
         position: 'relative',
         bottom: 0,
         justifyContent: "center",
         alignItems: "center",
         backgroundColor: "#083e0c",
      }} component="footer" square variant="outlined">
         <Container maxWidth="lg">
            <Box
               sx={{
                  flexGrow: 1,
                  justifyContent: "center",
                  alignItems: "center",
                  display: "flex",
                  mb: 2,
                  mt: 2,
               }}
            >
               <Typography variant="caption" color="initial">
                  {Copyright()}
               </Typography>
            </Box>
         </Container>
      </Paper>
   )
}

export default Footer;