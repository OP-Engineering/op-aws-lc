import { Text, View, StyleSheet } from 'react-native';
import { generateHmacKey } from 'op-aws-lc';

const key = generateHmacKey();

export default function App() {
  return (
    <View style={styles.container}>
      <Text>
        Hmac Key should have been generated. Hmac object is there?{' '}
        {!!key ? 'Yes' : 'No'}
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
