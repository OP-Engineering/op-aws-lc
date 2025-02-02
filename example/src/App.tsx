import { Text, View, StyleSheet } from 'react-native';
import { hmac } from 'op-aws-lc';

const key = hmac();

export default function App() {
  return (
    <View style={styles.container}>
      <Text>
        Hmac Key should have been generated and signed a hello message. Signed
        message is:
        {key}
      </Text>
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
