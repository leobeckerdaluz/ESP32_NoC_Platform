import React from 'react';
import { StyleSheet, Text, View, Image } from 'react-native';
import HabitosJson from '../../habitos';

export default class HabitosPage extends React.Component {
  renderList(){
    
    // // CRIAR UM ARRAY
    // const habitos = [
    //   'Correr', 'Musculação', 'Futiba', 'Basquete', 'fala aí'
    // ]

    // // RETORNAR UM COMPONENTE TEXT COM CADA HÁBITO DO ARRAY
    // const ret = habitos.map((habito, idx) => {
    //   return <Text key={idx}> {habito}</Text>
    // })
    
    const ret = HabitosJson.map((habito, idx) => {
      return <Text key={idx}> {habito.nome}</Text>
    })

    return ret;
  }
  
  render() {
    return (
      <View>
        {/* <Text>Sandy caio</Text> */}
        {this.renderList()}
        <Image
          style={{width: '100%',height: '100%'}}
          source={require('../images/sandy.jpg')}
        />
      </View>
    );
  }
}