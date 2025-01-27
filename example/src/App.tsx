import { Text, View, StyleSheet } from 'react-native';
import { hmac } from 'op-aws-lc';

export default function App() {
  return (
    <View style={styles.container}>
      <Text>Result: {hmac()}</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
});
