import React, { useEffect, useState } from 'react'
import axios from 'axios';
import Card from '@mui/material/Card';
import CardContent from '@mui/material/CardContent';
import CardMedia from '@mui/material/CardMedia';
import Typography from '@mui/material/Typography';
import { Button, CardActionArea, CardActions } from '@mui/material';

function Home() {
   // useState variables
   const [news, setNews] = useState([]);

   const getLatestNews = async () => {
      const response = await axios.get('https://newsapi.org/v2/everything?q=robotics&sortBy=publishedAt&apiKey=' + process.env.REACT_APP_CURRENT_NEWS_API);
      console.log(response.data['articles']);
      // store the news articles in state
      setNews(response.data['articles']);
   }


   useEffect(() => {
      getLatestNews();
   }, []);

   return (
      <div>
         <h2 style={{ textAlign: 'center' }}>Welcome to the MEM 455 Robotics Website</h2>
         <div style={{ display: 'flex', justifyContent: 'center', alignItems: 'center', flexDirection: 'column' }}>
            <h3>Robotics News</h3>
            <div className="newsContainer" style={{
               width: '80%',
               display: 'flex',
               justifyContent: 'left',
               alignItems: 'center',
               flexWrap: 'wrap',
               border: '1px solid black',
               height: '500px',
               overflow: 'hidden',
               overflowY: 'scroll',
            }}>

               {/* display news */}
               {news.map((article, index) => {
                  return (
                     <div key={index} style={{ display: 'flex', justifyContent: 'left', alignItems: 'center',  width: '100%'}}>
                        <Card style={{width:'100%', 'justifyContent': 'left'}}>
                           <CardActionArea href={article.url} style={{display: 'flex', justifyContent: 'left', alignItems: 'center', flexDirection: 'row'}}>
                              <CardMedia
                                 component="img"
                                 image={article.urlToImage}
                                 alt=""
                                 style={{width: '90px', height: '90px', borderRadius: '10%'}}
                              />
                              <CardContent>
                                 <Typography gutterBottom variant="h7" component="div">
                                    {article.title}
                                 </Typography>
                                 <Typography variant="body2" color="text.secondary">
                                    {article.description}
                                 </Typography>
                              </CardContent>
                           </CardActionArea>
                        </Card>
                     </div>
                  )
               })}
            </div>
         </div>
      </div>
   )
}

export default Home