import React from 'react';
import { StyleSheet, Text, View } from 'react-native';

export default class App extends React.Component {
  render() {
    return (
      <View style={styles.container}>
        <Text style={styles.titulo}> SÉEEENDY TÍTULO </Text>
        <Text style={styles.conteudo}>>SÉEEENDY CONTEUDO</Text>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
  titulo: {
    color: "red",
    fontSize: 16,
  },
  conteudo: {
    color: "black",
    fontSize: 12,
  },
});
