import './css/App.css';
import { BrowserRouter as Router, Route, Routes } from 'react-router-dom';

// import pages
import Home from './pages/Home';
import Controller from './pages/Controller';

// import components
import Footer from './components/Footer';
import Navbar from './components/Navbar';


function App() {
   return (
      <Router>
         <Navbar />
         <Routes>
            <Route path="/" exact element={<Home />} />
            <Route path="/controller" element={<Controller />} />
         </Routes>
         <Footer />
      </Router>
   );
}

export default App;
