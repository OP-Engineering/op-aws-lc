import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';

export interface Spec extends TurboModule {
  install(): string | undefined;
}

export default TurboModuleRegistry.getEnforcing<Spec>('OpAwsLc');
