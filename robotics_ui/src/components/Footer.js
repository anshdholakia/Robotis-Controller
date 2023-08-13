import React from 'react'
import Box from '@mui/material/Box';
import Typography from '@mui/material/Typography';
import Container from '@mui/material/Container';
import Link from '@mui/material/Link';

function Copyright() {
   return (
      <Typography variant="body2" color="text.secondary">
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
      // place box at the bottom of the page
      <Box
         component="footer"
         sx={{
            py: 3,
            px: 2,
            mt: 'auto',
            backgroundColor: (theme) =>
               theme.palette.mode === 'light'
                  ? theme.palette.grey[200]
                  : theme.palette.grey[800],
         }}
         style={{
            position: "relative",
            bottom: "0",
         }}
      >
         <Container maxWidth="sm">
            <Typography variant="body1">
               Robotics Controller - Drexel Engineering. Made by Ansh Dholakia
            </Typography>
            <Copyright />
         </Container>
      </Box>
   )
}

export default Footer;