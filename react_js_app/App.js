import {createAppContainer, createStackNavigator} from 'react-navigation';
import HabitosPage from './src/pages/HabitosPage';

const AppNavigator = createStackNavigator(
  {
    'Main':{
      screen: HabitosPage,
      navigationOptions:{
        title: 'Viva o Séndy!',
        headerTitleStyle:{
          textAlign:'left',
          fontSize: 30,
          color: 'orange',
        }
      }
    }
  },
  {
    defaultNavigationOptions: {
      title: 'séndy',
      // headerTintColor: 'white',
      headerStyle: {
        backgroundColor: 'green',
        borderBottomColor: 'black',
      },
      headerTitleStyle:{
        color: 'white',
        fontSize: 20,
        flexGrow: 1,
        textAlign: 'center',
      }
    }
  }
);

const AppContainer = createAppContainer(AppNavigator);

export default AppContainer;